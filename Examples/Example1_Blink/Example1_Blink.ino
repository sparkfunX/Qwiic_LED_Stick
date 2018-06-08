/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14641

  This example blinks the entire LED Stick.

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
  Serial.println("On");
  LEDstick.SetLEDColor(255, 255, 255); //Set all LEDs the same color (white)
  delay(1000);
  Serial.println("Off");
  LEDstick.LEDOff(); //Turn off all LEDs
  delay(1000);

}

