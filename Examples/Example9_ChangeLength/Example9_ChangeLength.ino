/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14641

  This example changes the length of the attached LED strip and shows the results by writing the whole strip white.

*/

#include "Qwiic_LED_Stick.h"

LED LEDStick;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  LEDStick.begin();
  LEDStick.changeLength(5);
  LEDStick.setLEDColor(10, 10, 10);

}

void loop() {


}

