/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14641

  This example makes a moving rainbow on the LED Stick.

*/

#include <Wire.h>

byte LEDAddress = 0x22;

void setup() {
  Wire.begin();
  Serial.begin(9600);

}
void loop() {
  WalkingRainbow(10,100);
}

//Walks a rainbow across LED strip of length LEDlength with a delay of delayTime
void WalkingRainbow(byte LEDlength, int delayTime) {
   changeLength(LEDlength);

  while (1) {
    for (byte j = 0; j < LEDlength; j++) {
      for (byte i = 1 ; i <= LEDlength ; i++) {
        int n = i - j;
        if (n <= 0) n += LEDlength;
        if (n <= LEDlength / 6) {
          SetLEDColor(i, 255, floor(6 * 255 / LEDlength * n), 0);
        }
        else if (n > LEDlength / 6 && n <= LEDlength / 3) {
          SetLEDColor(i, floor(510 - 6 * 255 / LEDlength * n), 255, 0);
        }
        else if (n > LEDlength / 3 && n <= LEDlength / 2) {
          SetLEDColor(i, 0, 255, floor( 6 * 255 / LEDlength * n) - 510);
        }
        else if (n > LEDlength / 2 && n <= 2 * LEDlength / 3) {
          SetLEDColor(i, 0, floor(1020 - 6 * 255 / LEDlength * n), 255);
        }
        else if (n > 2 * LEDlength / 3 && n <= 5 * LEDlength / 6) {
          SetLEDColor(i, floor(6 * 255 / LEDlength * n - 1020), 0, 255);
        }
        else {
          SetLEDColor(i, 255, 0, floor(1530 - 6 * 255 / LEDlength * n));
        }
      }
      delay(delayTime);
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
    Serial.println("Error: Sensor did not ack (SetLEDColor)");
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
    Serial.println("Error: Sensor did not ack (SetLEDBrightness(all))");
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
    Serial.println("Error: Sensor did not ack (ChangeAddress)");
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


