/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14641

  This example changes the address of the LED stick and shows the results by writing the whole strip white.

*/

#include <Wire.h>

byte LEDAddress = 0x23;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  changeAddress(0x23, 0x29);
  SetLEDColor(10,10,10);
}

void loop() {


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
    Serial.println("Error: Sensor did not ack");
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
    Serial.println("Error: Sensor did not ack");
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
  if (length = 255) return false; //Error check, otherwise for loop never ends
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
    Serial.println("Error: Sensor did not ack");
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
    Serial.println("Error: Sensor did not ack");
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
    Serial.println("Error: Sensor did not ack");
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
    Serial.println("Error: Sensor did not ack");
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
    Serial.println("Error: Sensor did not ack");
    return (false);
  }
  return (true);
}
