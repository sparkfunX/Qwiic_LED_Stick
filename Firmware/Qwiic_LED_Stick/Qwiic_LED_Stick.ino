/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Qwiic LED Stick is an I2C based LED Stick that stores an array of colors and brightness to write to an LED strip
  The Qwiic LED Stick features ten addressable APA102 LEDs, making it easy to add full color LED control using I2C.
  Write to individual LEDs to display a count in binary, or write to the whole strip for cool lighting effects. 

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14783

  To install support for ATtiny85 in Arduino IDE: https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md
  This core is installed from the Board Manager menu
  This core has built in support for I2C S/M and serial
  If you have Dave Mellis' ATtiny installed you may need to remove it from \Users\xx\AppData\Local\Arduino15\Packages

  To support 400kHz I2C communication reliably ATtiny85 needs to run at 8MHz. This requires user to
  click on 'Burn Bootloader' before code is loaded.
*/

#include <Wire.h>
#include <EEPROM.h>
#include <avr/sleep.h> //Needed for sleep_mode

#define LOCATION_I2C_ADDRESS (0x01) //Location in EEPROM where the I2C address is stored
#define LOCATION_LED_LENGTH (0x02) //Location in EEPROM where the LED_LENGTH is stored
#define I2C_ADDRESS_DEFAULT (0x23) //Default I2C address
#define I2C_ADDRESS_JUMPER (0x22) //Address with jumper closed 
#define LED_LENGTH (10) //Code supports LED strip up to length 100
#define LED_LENGTH_MAX (90) //Use an external power supply if adding LEDs 

#define COMMAND_CHANGE_ADDRESS (0xC7)
#define COMMAND_CHANGE_LED_LENGTH (0x70)
#define COMMAND_WRITE_SINGLE_LED_COLOR (0x71)
#define COMMAND_WRITE_ALL_LED_COLOR (0x72)
#define COMMAND_WRITE_RED_ARRAY (0x73)
#define COMMAND_WRITE_GREEN_ARRAY (0x74)
#define COMMAND_WRITE_BLUE_ARRAY (0x75)
#define COMMAND_WRITE_SINGLE_LED_BRIGHTNESS (0x76)
#define COMMAND_WRITE_ALL_LED_BRIGHTNESS (0x77)
#define COMMAND_WRITE_ALL_LED_OFF (0x78)


//Variables used in the I2C interrupt so we use volatile
volatile byte setting_i2c_address = I2C_ADDRESS_DEFAULT; //The 7-bit I2C address of this LEDstick
volatile byte setting_LED_length = LED_LENGTH; //The default length of the LED strip

//This struct keeps a record of the states of all LEDs
typedef struct {
  volatile byte red;
  volatile byte green;
  volatile byte blue;
  volatile byte brightness;
} LEDSettings;
LEDSettings LEDStrip[LED_LENGTH_MAX]; //creates a global array of the states of all LEDs

//  for ATtiny85
const byte addr = 1; //Addr jumper
const byte dataPin = 3; //pin to data line of LEDs
const byte clkPin = 4; //pin to clock line of LEDs

void setup() {
  pinMode(addr, INPUT_PULLUP);
  pinMode(dataPin, OUTPUT); //Data out to the LEDs
  pinMode(clkPin, OUTPUT); //clkPin for LED data shift
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  readSystemSettings(); //Load all system settings from EEPROM
  initializeLEDArray();
  startI2C(); //Determine the I2C address we should be using and begin listening on I2C bus
}

void loop() {
  WriteLED(); //Write the current state of the LED array to the LED strip
  sleep_mode(); //Stop everything and go to sleep. Wake up if I2C event occurs.
}

//When LEDStick receives data bytes, this function is called as an interrupt
//Can initiate commands to change color/brightness/length/I2C address
void receiveEvent(int numberOfBytesReceived)
{
  while (Wire.available())
  {
    //Record bytes to local array
    byte incoming = Wire.read();

    if (incoming == COMMAND_CHANGE_ADDRESS) //Set new I2C address
    {
      if (Wire.available())
      {
        setting_i2c_address = Wire.read();

        //Error check
        if (setting_i2c_address < 0x08 || setting_i2c_address > 0x77)
          continue; //Command failed. This address is out of bounds.

        EEPROM.write(LOCATION_I2C_ADDRESS, setting_i2c_address);

        //Our I2C address may have changed because of user's command
        startI2C(); //Determine the I2C address we should be using and begin listening on I2C bus
      }
    }
    else if (incoming == COMMAND_CHANGE_LED_LENGTH ) //Change how many LEDs are in your strip
    {
      if (Wire.available())
      {
        byte temp_LED_length = Wire.read();
        //Error check
        if (temp_LED_length < 1 || temp_LED_length > LED_LENGTH_MAX)
          continue; //Command failed. This length is not supported.
        setting_LED_length = temp_LED_length;
        EEPROM.write(LOCATION_LED_LENGTH, setting_LED_length);
      }
    }
    else if (incoming == COMMAND_WRITE_SINGLE_LED_COLOR) //Change color one LED
    {
      if (Wire.available())
      {
        byte number = Wire.read();
        if (number < 1 || number > LED_LENGTH_MAX)
          continue; //error check, cannot write to array indexed at locations <0 or >max index
        LEDStrip[number - 1].red = Wire.read();
        LEDStrip[number - 1].green = Wire.read();
        LEDStrip[number - 1].blue = Wire.read();
      }
    }
    else if (incoming == COMMAND_WRITE_ALL_LED_COLOR) { //Change color all LEDs same
      if (Wire.available())
      {
        LEDStrip[0].red = Wire.read();
        LEDStrip[0].green = Wire.read();
        LEDStrip[0].blue = Wire.read();
        for (byte i = 0; i < setting_LED_length; i++) {
          LEDStrip[i].red = LEDStrip[0].red;
          LEDStrip[i].green = LEDStrip[0].green;
          LEDStrip[i].blue = LEDStrip[0].blue;
        }
      }
    }
    //The following three commands are to update the LED with different colors
    //for each LED in a single function call from the library
    else if (incoming == COMMAND_WRITE_RED_ARRAY) { // writes to the red array 
      if (Wire.available())
      {
        byte Length = Wire.read();
        byte offset = Wire.read();
        for (byte i = 0; i < Length && i < setting_LED_length; i++) {
          LEDStrip[i + offset].red = Wire.read();
        }
      }
    }
    else if (incoming == COMMAND_WRITE_GREEN_ARRAY) { //writes to the green array
      if (Wire.available())
      {
        byte Length = Wire.read();
        byte offset = Wire.read();
        for (byte i = 0; i < Length && i < setting_LED_length; i++) {
          LEDStrip[i + offset].green = Wire.read();
        }
      }
    }
    else if (incoming == COMMAND_WRITE_BLUE_ARRAY) { //writes to the blue array
      if (Wire.available())
      {
        byte Length = Wire.read();
        byte offset = Wire.read();
        for (byte i = 0; i < Length && i < setting_LED_length; i++) {
          LEDStrip[i + offset].blue = Wire.read();
        }
      }
    }
    else if (incoming == COMMAND_WRITE_SINGLE_LED_BRIGHTNESS) { //change brightness of single LED
      if (Wire.available())
      {
        byte number = Wire.read();
        if (number < 1 || number > LED_LENGTH_MAX)
          continue; //error check, cannot write to array indexed at locations <0 or >max index
        LEDStrip[number - 1].brightness = Wire.read();
      }
    }
    else if (incoming == COMMAND_WRITE_ALL_LED_BRIGHTNESS) { //change brightness of all LEDs (same)
      if (Wire.available())
      {
        LEDStrip[0].brightness = Wire.read();
        for (byte i = 0; i < setting_LED_length; i++)
          LEDStrip[i].brightness = LEDStrip[0].brightness;
      }
    }
    else if (incoming == COMMAND_WRITE_ALL_LED_OFF) {
      for (byte i = 0; i < setting_LED_length; i++) {
        LEDStrip[i].red = 0;
        LEDStrip[i].green = 0;
        LEDStrip[i].blue = 0;
      }
    }
  }
}

//Shifts out information from the LED settings array to the LEDs
void WriteLED(void) {
  for (byte i = 0; i < 4; i++) { //start frame is 4 bytes of 0
    shiftOut(dataPin, clkPin, MSBFIRST, (byte)0);
  }
  //LED frame starts with 0b111, is followed by 5 bit brightness value, then bytes for blue, green, red
  for (byte i = 0; i < setting_LED_length; i++) { 
    shiftOut(dataPin, clkPin, MSBFIRST, (0b11100000) | LEDStrip[i].brightness);
    shiftOut(dataPin, clkPin, MSBFIRST, LEDStrip[i].blue);
    shiftOut(dataPin, clkPin, MSBFIRST, LEDStrip[i].green);
    shiftOut(dataPin, clkPin, MSBFIRST, LEDStrip[i].red);
  }
  for (byte i = 0; i < 3; i++) { //stop frame is 3 bytes of 0 followed by 1 byte 1
    shiftOut(dataPin, clkPin, MSBFIRST, (byte)0);
  }
  shiftOut(dataPin, clkPin, MSBFIRST, (byte)1);
}

//Reads the current system settings from EEPROM
//If anything looks weird, reset setting to default value
void readSystemSettings(void)
{
  //Read what I2C address we should use
  setting_i2c_address = EEPROM.read(LOCATION_I2C_ADDRESS);
  if (setting_i2c_address == 255)
  {
    setting_i2c_address = I2C_ADDRESS_DEFAULT; //By default, we listen for I2C_ADDRESS_DEFAULT
    EEPROM.write(LOCATION_I2C_ADDRESS, setting_i2c_address);
  }
  //Read what I2C address we should use
  setting_LED_length = EEPROM.read(LOCATION_LED_LENGTH);
  if (setting_LED_length == 255)
  {
    setting_LED_length = LED_LENGTH;  //By default LED strip is LED_LENGTH long
    EEPROM.write(LED_LENGTH, setting_LED_length);
  }
}

//Sets all color values in LEDSettings array to 0 and brightness to maximum of 31
void initializeLEDArray(void) {
  for (byte i = 0; i < LED_LENGTH_MAX; i++) {
    LEDStrip[i].red = 0;
    LEDStrip[i].green = 0;
    LEDStrip[i].blue = 0; //colors start at zero so the LED starts OFF
    LEDStrip[i].brightness = 31; //brightness starts at maximuminitializeLEDArray(LEDStrip[i]);  //Set all values
  }
}

//Begin listening on I2C bus as I2C slave using the global variable setting_i2c_address
void startI2C()
{
  Wire.end(); //Before we can change addresses we need to stop

  if (digitalRead(addr) == HIGH) //Default is HIGH, the jumper is open
    Wire.begin(setting_i2c_address); //Start I2C and answer calls using address from EEPROM
  else
    Wire.begin(I2C_ADDRESS_JUMPER); //Force address to I2C_ADDRESS_JUMPER if user has soldered jumper

  //The connections to the interrupts are severed when a Wire.begin occurs. So re-declare them.
  Wire.onReceive(receiveEvent);
}
