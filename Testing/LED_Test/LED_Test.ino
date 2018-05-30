#include <Wire.h>

#define COMMAND_CHANGE_ADDRESS (0xC7)
#define COMMAND_CHANGE_LED_LENGTH (0x70)
#define COMMAND_WRITE_SINGLE_LED_COLOR (0x71)
#define COMMAND_WRITE_ALL_LED_COLOR (0x72)
#define COMMAND_WRITE_ALL_LED_UNIQUE_COLOR (0x73)
#define COMMAND_WRITE_SINGLE_LED_BRIGHTNESS (0x74)
#define COMMAND_WRITE_ALL_LED_BRIGHTNESS (0x75)
#define COMMAND_WRITE_ALL_LED_OFF (0x76)


byte LEDAddress = 0x23;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("LED TEST");
  //SetLEDColor(255, 255, 255);
}

void loop() {
  SetLEDColor(10, 10, 10);
  delay(1000);
  for (int i = 1; i <= 10; i++) { //individually controls color
    SetLEDColor(i, 10, i, 10 - i);
    delay(100);
  }
  for (int i = 1; i <= 10; i++) { //individually controls brightness
    SetLEDBrightness(i, 31 - 2 * i);
    delay(200);
  }
  LEDOff();
  delay(1000);
  for (int i = 1; i <= 10; i++) { //adjusting brightness after turned off does nothing
    SetLEDBrightness(i, 31 - 2 * i);
    delay(100);
  }  //6 seconds
  for (int i = 1; i <= 10; i++) { //brightness of all increses as number of lit LEDs increases
    SetLEDColor(i, 10 - i, 10, i);
    SetLEDBrightness(2 * (i + 1));
    delay(1000);
  } //16 seconds
  SetLEDColor(0, 10, 10);
  delay(1000);
  LEDOff();
  delay(3000);
  //20 seconds
}
//Change the color of a specific LED
//each color must be a value between 0-255
//LEDS indexed starting at 1
boolean SetLEDColor(byte number, byte red, byte green, byte blue) {
  Wire.beginTransmission(LEDAddress);
  Wire.write(0x71);
  Wire.write(number);
  Wire.write(red);
  Wire.write(green);
  Wire.write(blue);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (SetLEDColor)");
    return (false);
  }
  return (true);
}
//Change the color of all LEDs
//each color must be a value between 0-255
boolean SetLEDColor(byte red, byte green, byte blue) {
  Wire.beginTransmission(LEDAddress);
  Wire.write(0x72);
  Wire.write(red);
  Wire.write(green);
  Wire.write(blue);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack(SetLEDColor(all))");
    return (false);
  }
  return (true);
}
//Change the color of all LEDs at once to individual values
//Pass in 3 arrays of color values
//each color must be a value between 0-255
//length must be less than 255
boolean SetLEDColor(byte * red, byte * green, byte * blue, byte length) {
  Wire.beginTransmission(LEDAddress);
  Wire.write(0x73);
  Wire.write(length);
  if (length=255) return false; //Error check, otherwise for loop never ends
  for (byte i = 0; i < length; i++) {
    Wire.write(red[i]);
    Wire.write(green[i]);
    Wire.write(blue[i]);
  }
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack(SetLEDColor(all))");
    return (false);
  }
  return (true);
}

//Change the brightness of a specific LED, while keeping their current color
//brightness must be a value between 0-31
//To turn LEDs off but remember their previous color, set brightness to 0
//LEDS indexed starting at 1
boolean SetLEDBrightness(byte number, byte brightness) {
  Wire.beginTransmission(LEDAddress);
  Wire.write(0x74);
  Wire.write(number);
  Wire.write(brightness);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack(SetLEDBrightness)");
    return (false);
  }
  return (true);
}
//Change the brightness of all LEDs, while keeping their current color
//brightness must be a value between 0-31
//To turn all LEDs off but remember their previous color, set brightness to 0
boolean SetLEDBrightness(byte brightness) {
  Wire.beginTransmission(LEDAddress);
  Wire.write(0x75);
  Wire.write(brightness);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (SetLEDBrightness(all))");
    return (false);
  }
  return (true);
}
//Turn all LEDS off by setting color to 0
boolean LEDOff(void) {
  Wire.beginTransmission(LEDAddress);
  Wire.write(0x76);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (LEDOff)");
    return (false);
  }
  return (true);
}
//Change the I2C address from one address to another
boolean changeAddress(byte oldAddress, byte newAddress)
{
  Wire.beginTransmission(oldAddress); //Communicate using the old address
  Wire.write(0xC7); //0xC7 is the register location on the KeyPad to change its I2C address
  Wire.write(newAddress); //Go to the new address
  LEDAddress = newAddress;
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (ChangeAddress)");
    return (false);
  }
  return (true);
}
//Change the length of LEDs
boolean changeLength(byte newLength)
{
  Wire.beginTransmission(LEDAddress); //Communicate using the old address
  Wire.write(0x70); //0x70 is the register location on the LEDStick to change the length
  Wire.write(newLength); //Update the length
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (ChangeLength)");
    return (false);
  }
  return (true);
}
