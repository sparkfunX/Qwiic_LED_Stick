/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14641

  This example controls single LEDs on the LED Stick.

*/

#include <Wire.h>
#include "Qwiic_LED_Stick.h"

LED LEDStick;

void setup() {
  Serial.begin(9600);
  LEDStick.begin();
}

void loop() {
  LEDStick.LEDOff(); //turn off all LEDs
  LEDStick.setLEDColor(4,255, 0, 0); //turn on LED#4, red
  delay(1000);
  LEDStick.LEDOff(); //turn off all LEDs
  LEDStick.setLEDColor(6,255, 0, 0); //turn on LED#6, red
  delay(1000);

}

