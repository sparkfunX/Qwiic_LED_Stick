/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14641

  This example counts up 10 bits of binary and displays the number on the LED Stick.

*/

#include <Wire.h>

byte LEDAddress = 0x22;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  for (int count = 0; count < 1024; count++) {
    binaryLEDDisplay(count, 10);
    binarySerialDisplay(count, 10);
    delay(1000);
  }

}

//Display binary on LEDS (LSB==LED1) of length LEDLength
void binaryLEDDisplay(int count, byte LEDLength) {
  for (byte i = 0; i < LEDLength; i++) {
    SetLEDColor(i + 1, 255 * ((count & 1 << i) != 0), 0, 0); //ith LED will be red if the ith bit of the count is not zero
  }
}

//Serial.print decimal and binary value of count, expecting n bits
void binarySerialDisplay(int count, byte n) {
  Serial.print(count);
  Serial.print("\t|\t");
  for (byte i = n; i > 0 ; i--) { //Start printing at MSB
    Serial.print((count & 1 << (i - 1)) != 0);  //ith bit will be 1 if the ith bit of the count is not zero, and 0 if it is 0
  }
  Serial.print("\n"); //new line
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

