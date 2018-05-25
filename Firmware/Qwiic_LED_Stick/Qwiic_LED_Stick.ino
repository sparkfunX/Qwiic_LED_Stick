#include <Wire.h>
#include <EEPROM.h>
#include <avr/sleep.h> //Needed for sleep_mode

#define LOCATION_LED_LENGTH 0x02
#define LOCATION_I2C_ADDRESS 0x01 //Location in EEPROM where the I2C address is stored
#define I2C_ADDRESS_DEFAULT (0x23)
#define I2C_ADDRESS_JUMPER (0x22)
#define LED_LENGTH (10) //Code supports up to length 222, though board may not necessarily handle current required
#define LED_LENGTH_MAX (90)

#define COMMAND_CHANGE_ADDRESS (0xC7)
#define COMMAND_CHANGE_LED_LENGTH (0x69)
#define COMMAND_WRITE_SINGLE_LED_COLOR (0x70)
#define COMMAND_WRITE_ALL_LED_COLOR (0x71)
#define COMMAND_WRITE_SINGLE_LED_BRIGHTNESS (0x72)
#define COMMAND_WRITE_ALL_LED_BRIGHTNESS (0x73)
#define COMMAND_WRITE_ALL_LED_OFF (0x74)


//#define ATTINY85
//#ifdef ATTINY85
//#define DATAPIN (3)
//#define CLK (4)
//#endif
#define ARDUINOUNO
#ifdef ARDUINOUNO
#define DATAPIN (11)
#define CLK (13)
#endif

//Variables used in the I2C interrupt so we use volatile
volatile byte setting_i2c_address = I2C_ADDRESS_DEFAULT; //The 7-bit I2C address of this LEDstick
volatile byte setting_LED_length = LED_LENGTH; //The default length of the LED strip
//This struct keeps a record of the states of all LEDs
typedef struct {
  volatile byte red;
  volatile byte green;
  volatile byte blue;
  volatile byte brightness;
} color;
color LEDStrip[LED_LENGTH_MAX];
const byte addr = 9; //Addr jumper

void setup() {
  pinMode(DATAPIN, OUTPUT); //Data out to the LEDs
  pinMode(CLK, OUTPUT); //CLK for LED data shift
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  readSystemSettings(); //Load all system settings from EEPROM
  for (byte i = 0; i < LED_LENGTH_MAX; i++) {
    initializeColorArray(LEDStrip[i]);
  }
  startI2C(); //Determine the I2C address we should be using and begin listening on I2C bus
}

void loop() {
  WriteLED();
  sleep_mode(); //Stop everything and go to sleep. Wake up if I2C event occurs.
}

//When LEDStrip receives data bytes, this function is called as an interrupt
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
        if (temp_LED_length > LED_LENGTH_MAX)
          continue; //Command failed. This length is not supported.
        setting_LED_length = temp_LED_length;
        EEPROM.write(LOCATION_LED_LENGTH, setting_LED_length);

      }
    }
    else if (incoming == COMMAND_WRITE_SINGLE_LED_COLOR) //Change color LEDS
    {
      if (Wire.available())
      {
        byte number = Wire.read();
        LEDStrip[number].red = Wire.read();
        LEDStrip[number].green = Wire.read();
        LEDStrip[number].blue = Wire.read();
      }
    }
    else if (incoming == COMMAND_WRITE_ALL_LED_COLOR) {
      if (Wire.available())
      {
        byte red = Wire.read();
        byte green = Wire.read();
        byte blue = Wire.read();
        for (byte i = 0; i < setting_LED_length; i++) {
          LEDStrip[i].red = red;
          LEDStrip[i].green = green;
          LEDStrip[i].blue = blue;
        }
      }
    }
    else if (incoming == COMMAND_WRITE_SINGLE_LED_BRIGHTNESS) {
      if (Wire.available())
      {
        byte number = Wire.read();
        LEDStrip[number].brightness = Wire.read();
      }
    }
    else if (incoming == COMMAND_WRITE_ALL_LED_BRIGHTNESS) {
      if (Wire.available())
      {
        byte brightness = Wire.read();
        for (byte i = 0; i < setting_LED_length; i++)
          LEDStrip[i].brightness = brightness;
      }
    }
    else if (incoming == COMMAND_WRITE_ALL_LED_OFF) {
      for (byte i = 0; i < setting_LED_length; i++) {
        LEDStrip[i].red &= 0;
        LEDStrip[i].green &= 0;
        LEDStrip[i].blue &= 0;
      }
    }
  }
}
void WriteLED(void) {
  for (byte i = 0; i < 4; i++) { //start frame is 4 bytes of 0
    shiftOut(DATAPIN, CLK, MSBFIRST, (byte)0);
  }
  for (byte i = 0; i < setting_LED_length; i++) { //LED frame starts with 0b111, is followed by 5 bit brightness value, then bytes for blue, green, red
    shiftOut(DATAPIN, CLK, MSBFIRST, (0b11100000) | LEDStrip[i].brightness);
    shiftOut(DATAPIN, CLK, MSBFIRST, LEDStrip[i].blue);
    shiftOut(DATAPIN, CLK, MSBFIRST, LEDStrip[i].green);
    shiftOut(DATAPIN, CLK, MSBFIRST, LEDStrip[i].red);
  }
  for (byte i = 0; i < 4; i++) { //stop frame is 3 bytes of 0 followed by 1 byte 1
    shiftOut(DATAPIN, CLK, MSBFIRST, (byte)0xFF);
  }
  //  shiftOut(DATAPIN, CLK, MSBFIRST, (byte)1);
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
void initializeColorArray(color colorPtr) {
  (colorPtr).brightness = 31; //brightness starts at maximum
  (colorPtr).red = 0;
  (colorPtr).green = 0;
  (colorPtr).blue = 0; //colors start at zero so the LED starts OFF
}
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
