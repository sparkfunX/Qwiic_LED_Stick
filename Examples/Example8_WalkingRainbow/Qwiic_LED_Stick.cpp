#include "Qwiic_LED_Stick.h"

 LEDStick::LEDStick(){
	_LEDAddress=0;
}

boolean LEDStick::begin(byte address){
	if (address<0x07||address>0x77) return false;
	_LEDAddress=address;
	Wire.begin();
	Wire.setClock(400000);
	return true;
}
//Change the color of a specific LED
//each color must be a value between 0-255
//LEDS indexed starting at 1
boolean LEDStick::SetLEDColor(byte number, byte red, byte green, byte blue) {
  Wire.beginTransmission(_LEDAddress);
  Wire.write(COMMAND_WRITE_SINGLE_LED_COLOR);
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
//Change the color of all LEDs
//each color must be a value between 0-255
boolean LEDStick::SetLEDColor(byte red, byte green, byte blue) {
  Wire.beginTransmission(_LEDAddress);
  Wire.write(COMMAND_WRITE_ALL_LED_COLOR);
  Wire.write(red);
  Wire.write(green);
  Wire.write(blue);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack(SetLEDColor(all))");
    return (false);
  }
  return (true);
}
//Change the color of all LEDs at once to individual values
//Pass in 3 arrays of color values
//each color must be a value between 0-255
//length must be less than 255
boolean LEDStick::SetLEDColor(byte * red, byte * green, byte * blue, byte length) {
  Wire.beginTransmission(_LEDAddress);
  Wire.write(COMMAND_WRITE_ALL_LED_UNIQUE_COLOR);
  Wire.write(length);
  for (byte i = 0; i < length; i++) {
    Wire.write(red[i]);
    Wire.write(green[i]);
    Wire.write(blue[i]);
  }
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack(SetLEDColor(all unique))");
    return (false);
  }
  return (true);
}

//Change the brightness of a specific LED, while keeping their current color
//brightness must be a value between 0-31
//To turn LEDs off but remember their previous color, set brightness to 0
//LEDS indexed starting at 1
boolean LEDStick::SetLEDBrightness(byte number, byte brightness) {
  Wire.beginTransmission(_LEDAddress);
  Wire.write(COMMAND_WRITE_SINGLE_LED_BRIGHTNESS);
  Wire.write(number);
  Wire.write(brightness);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack(SetLEDBrightness)");
    return (false);
  }
  return (true);
}
//Change the brightness of all LEDs, while keeping their current color
//brightness must be a value between 0-31
//To turn all LEDs off but remember their previous color, set brightness to 0
boolean LEDStick::SetLEDBrightness(byte brightness) {
  Wire.beginTransmission(_LEDAddress);
  Wire.write(COMMAND_WRITE_ALL_LED_BRIGHTNESS);
  Wire.write(brightness);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (SetLEDBrightness(all))");
    return (false);
  }
  return (true);
}

//Change the brightness of all LEDs at once to individual values
//brightness must be a value between 0-31
//To turn LEDs off but remember their previous color, set brightness to 0
boolean LEDStick::SetLEDBrightness(byte * brightness, byte length) {
  Wire.beginTransmission(_LEDAddress);
  Wire.write(COMMAND_WRITE_ALL_LED_UNIQUE_BRIGHTNESS);
  Wire.write(length);
  for (byte i = 0; i < length; i++) {
    Wire.write(brightness[i]);
  }

  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (SetLEDBrightness(all))");
    return (false);
  }
  return (true);
}
//Turn all LEDS off by setting color to 0
boolean LEDStick::LEDOff(void) {
  Wire.beginTransmission(_LEDAddress);
  Wire.write(COMMAND_WRITE_ALL_LED_OFF);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (LEDOff)");
    return (false);
  }
  return (true);
}
//Change the I2C address from one address to another
boolean LEDStick::changeAddress(byte oldAddress, byte newAddress)
{
  Wire.beginTransmission(oldAddress); //Communicate using the old address
  Wire.write(COMMAND_CHANGE_ADDRESS); //0xC7 is the register location on the KeyPad to change its I2C address
  Wire.write(newAddress); //Go to the new address
  _LEDAddress = newAddress;
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (ChangeAddress)");
    return (false);
  }
  return (true);
}
//Change the length of LEDs
boolean LEDStick::changeLength(byte newLength)
{
  Wire.beginTransmission(_LEDAddress); //Communicate using the old address
  Wire.write(COMMAND_CHANGE_LED_LENGTH); //0x70 is the register location on the LEDStick to change the length
  Wire.write(newLength); //Update the length
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (ChangeLength)");
    return (false);
  }
  return (true);
}
