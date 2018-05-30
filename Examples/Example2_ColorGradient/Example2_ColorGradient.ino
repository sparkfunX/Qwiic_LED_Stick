/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14641

  This example will display a linear gradient from one color to another on the LED Stick.

*/

#include <Wire.h>

byte LEDAddress = 0x23;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  byte r1 = 23, g1 = 178, b1 = 10, r2 = 64, g2 = 176, b2 = 216;
  colorGradient(r1, g1, b1, r2, g2, b2, 10);
}

void loop() {

}

//Display a linear gradient from color 1 (r1,g1,b1) to color 2 (r2,g2,b2) on LED strip of length LEDLength
void colorGradient(byte r1, byte g1, byte b1, byte r2, byte g2, byte b2, byte LEDLength) {
  int rStep = 0, gStep = 0, bStep = 0;
  boolean r = true, g = true, b = true; //if true, r/g/b2  > r/g/b 1
  if (r1 < r2)
    rStep = floor((r2 - r1) / LEDLength);
  else {
    r = false;
    rStep = floor((r1 - r2) / LEDLength);
  }
  if (g1 < g2)
    gStep = floor((g2 - g1) / LEDLength);
  else {
    g = false;
    gStep = floor((g1 - g2) / LEDLength);
  }
  if (b1 < b2)
    bStep = floor((b2 - b1) / LEDLength);
  else {
    b = false;
    bStep = floor((b1 - b2) / LEDLength);
  }
  if (!r && !g && !b) {
    for (byte i = 1; i <= LEDLength; i++) {
      SetLEDColor(i, r1 - rStep * i, g1 - gStep * i, b1 - bStep * i);
    }
  }
  else if (!r && !g && b) {
    for (byte i = 1; i <= LEDLength; i++) {
      SetLEDColor(i, r1 - rStep * i, g1 - gStep * i, b1 + bStep * i);
    }
  }
  else if (!r && g && !b) {
    for (byte i = 1; i <= LEDLength; i++) {
      SetLEDColor(i, r1 - (rStep * i), g1 + (gStep * i), b1 - (bStep * i));
    }
  }
  else if (r && !g && !b) {
    for (byte i = 1; i <= LEDLength; i++) {
      SetLEDColor(i, r1 + rStep * i, g1 - gStep * i, b1 - bStep * i);
    }
  }
  else if (r && g && !b) {
    for (byte i = 1; i <= LEDLength; i++) {
      SetLEDColor(i, r1 + rStep * i, g1 + gStep * i, b1 - bStep * i);
    }
  }
  else if (r && !g && b) {
    for (byte i = 1; i <= LEDLength; i++) {
      SetLEDColor(i, r1 + rStep * i, g1 - gStep * i, b1 + bStep * i);
    }
  }
  else if (!r && g && b) {
    for (byte i = 1; i <= LEDLength; i++) {
      SetLEDColor(i, r1 - rStep * i, g1 + gStep * i, b1 + bStep * i);
    }
  }
  else if (r && g && b) {
    for (byte i = 1; i <= LEDLength; i++) {
      SetLEDColor(i, r1 + rStep * i, g1 + gStep * i, b1 + bStep * i);
    }
  }
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
//Change the brightness of a specific LED, while keeping their current color
//brightness must be a value between 0-31
//To turn LEDs off but remember their previous color, set brightness to 0
//LEDS indexed starting at 1
boolean SetLEDBrightness(byte number, byte brightness) {
  Wire.beginTransmission(LEDAddress);
  Wire.write(0x73);
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
  Wire.write(0x74);
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
  Wire.write(0x75);
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
