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

#include "Qwiic_LED_Stick.h"

LED LEDStick;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  LEDStick.begin();
  byte r1 = 238, g1 = 49, b1 = 36, r2 = 66, g2 = 235, b2 = 244;
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
      LEDStick.setLEDColor(i, r1 - rStep * i, g1 - gStep * i, b1 - bStep * i);
    }
  }
  else if (!r && !g && b) {
    for (byte i = 1; i <= LEDLength; i++) {
      LEDStick.setLEDColor(i, r1 - rStep * i, g1 - gStep * i, b1 + bStep * i);
    }
  }
  else if (!r && g && !b) {
    for (byte i = 1; i <= LEDLength; i++) {
      LEDStick.setLEDColor(i, r1 - (rStep * i), g1 + (gStep * i), b1 - (bStep * i));
    }
  }
  else if (r && !g && !b) {
    for (byte i = 1; i <= LEDLength; i++) {
      LEDStick.setLEDColor(i, r1 + rStep * i, g1 - gStep * i, b1 - bStep * i);
    }
  }
  else if (r && g && !b) {
    for (byte i = 1; i <= LEDLength; i++) {
      LEDStick.setLEDColor(i, r1 + rStep * i, g1 + gStep * i, b1 - bStep * i);
    }
  }
  else if (r && !g && b) {
    for (byte i = 1; i <= LEDLength; i++) {
      LEDStick.setLEDColor(i, r1 + rStep * i, g1 - gStep * i, b1 + bStep * i);
    }
  }
  else if (!r && g && b) {
    for (byte i = 1; i <= LEDLength; i++) {
      LEDStick.setLEDColor(i, r1 - rStep * i, g1 + gStep * i, b1 + bStep * i);
    }
  }
  else if (r && g && b) {
    for (byte i = 1; i <= LEDLength; i++) {
      LEDStick.setLEDColor(i, r1 + rStep * i, g1 + gStep * i, b1 + bStep * i);
    }
  }
}

