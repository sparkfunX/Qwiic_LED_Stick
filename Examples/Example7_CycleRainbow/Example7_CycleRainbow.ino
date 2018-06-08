/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14641

  This example makes the LED Stick smoothly change through the colors of the rainbow.

*/

#include <Wire.h>
#include "Qwiic_LED_Stick.h"

byte LEDAddress = 0x23;
LEDStick LEDstick;

void setup() {
  Serial.begin(9600);
  LEDstick.begin();
}

void loop() {
  CycleRainbow(10);
}

//Cycle through the rainbow with all LEDs the same color
void CycleRainbow(int delayTime) {
  for (byte g = 0; g < 255; g++) {
    LEDstick.SetLEDColor(255, g, 0);
    delay(delayTime);
  }
  for (byte r = 255; r > 0; r--) {
    LEDstick.SetLEDColor(r, 255, 0);
    delay(delayTime);
  }
  for (byte b = 0; b < 255; b++) {
    LEDstick.SetLEDColor(0, 255, b);
    delay(delayTime);
  }
  for (byte g = 255; g > 0; g--) {
    LEDstick.SetLEDColor(0, g, 255);
    delay(delayTime);
  }
  for (byte r = 0; r < 255; r++) {
    LEDstick.SetLEDColor(r, 0, 255);
    delay(delayTime);
  }
  for (byte b = 255; b > 0; b--) {
    LEDstick.SetLEDColor(255, 0, b);
    delay(delayTime);
  }
}

