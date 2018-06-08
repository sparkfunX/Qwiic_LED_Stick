/*
  An I2C based LED Stick
  By: Ciara Jekel
  SparkFun Electronics
  Date: May 29th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Qwiic LED Stick is an I2C based LED Stick that stores an array of colors and brightness to write to an LED strip

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14641

  To install support for ATtiny85 in Arduino IDE: https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md
  This core is installed from the Board Manager menu
  This core has built in support for I2C S/M and serial
  If you have Dave Mellis' ATtiny installed you may need to remove it from \Users\xx\AppData\Local\Arduino15\Packages

  To support 400kHz I2C communication reliably ATtiny85 needs to run at 8MHz. This requires user to
  click on 'Burn Bootloader' before code is loaded.

*/

#ifndef Qwiic_LED_Stick_h
#define Qwiic_LED_Stick_h

#include "Arduino.h"
#include <Wire.h>

#define COMMAND_CHANGE_ADDRESS (0xC7)
#define COMMAND_CHANGE_LED_LENGTH (0x70)
#define COMMAND_WRITE_SINGLE_LED_COLOR (0x71)
#define COMMAND_WRITE_ALL_LED_COLOR (0x72)
#define COMMAND_WRITE_ALL_LED_UNIQUE_COLOR (0x73)
#define COMMAND_WRITE_SINGLE_LED_BRIGHTNESS (0x74)
#define COMMAND_WRITE_ALL_LED_BRIGHTNESS (0x75)
#define COMMAND_WRITE_ALL_LED_UNIQUE_BRIGHTNESS (0x76)
#define COMMAND_WRITE_ALL_LED_OFF (0x77)


class LEDStick
{
    // user-accessible "public" interface
  public:
    LEDStick();
    boolean begin(byte address = 0x23);
    boolean SetLEDColor(byte number, byte red, byte green, byte blue);
    boolean SetLEDColor(byte red, byte green, byte blue);
    boolean SetLEDColor(byte * red, byte * green, byte * blue, byte length);
    boolean SetLEDBrightness(byte number, byte brightness);
    boolean SetLEDBrightness(byte brightness);
    boolean SetLEDBrightness(byte * brightness, byte length);
    boolean LEDOff(void);
    boolean changeAddress(byte oldAddress, byte newAddress);
    boolean changeLength(byte newLength);
  private:
    byte _LEDAddress;
};



#endif
