#include <Wire.h>
#define LEDAddress (34)
void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("LED TEST");
  SetLEDColor(255, 255, 255);

}

void loop() {
  SetLEDColor(10, 10, 10);
  delay(1000);
  for (int i = 0; i < 10; i++) { //individually controls color
    SetLEDColor(i, 10, i, 10 - i);
    delay(100);
  }
  for (int i = 0; i < 10; i++) { //individually controls brightness
    SetLEDBrightness(i, 31 - 2 * i);
    delay(200);
  }
  LEDOff();
  changeAddress(34, 55);
  delay(1000);
  for (int i = 0; i < 10; i++) { //adjusting brightness after turned off does nothing
    SetLEDBrightness(i, 31 - 2 * i);
    delay(100);
  }  //6 seconds
  for (int i = 0; i < 10; i++) { //brightness of all increses as number of lit LEDs increases
    SetLEDColor(i, 10 - i, 10, i);
    SetLEDBrightness(2 * (i + 1));
    delay(1000);
  } //16 seconds
  SetLEDColor(0, 10, 10);
  delay(1000);
  LEDOff();
  changeAddress(55, 34);
  delay(3000);
  //20 seconds
}
//each color must be a value between 0-255
boolean SetLEDColor(byte number, byte red, byte green, byte blue) {
  Wire.beginTransmission((uint8_t)LEDAddress);
  Wire.write(0x70);
  Wire.write(number);
  Wire.write(red);
  Wire.write(blue);
  Wire.write(green);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (SetLEDColor)");
    return (false);
  }
  return (true);
}
//each color must be a value between 0-255
boolean SetLEDColor(byte red, byte green, byte blue) {
  Wire.beginTransmission((uint8_t)LEDAddress);
  Wire.write(0x71);
  Wire.write(red);
  Wire.write(blue);
  Wire.write(green);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack(SetLEDColor(all))");
    return (false);
  }
  return (true);

}
//brightness must be a value between 0-31
boolean SetLEDBrightness(byte number, byte brightness) {
  Wire.beginTransmission((uint8_t)LEDAddress);
  Wire.write(0x72);
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
//brightness must be a value between 0-31
//if you want to turn the LEDs off but remember their previous color, set brightness to 0
boolean SetLEDBrightness(byte brightness) {
  Wire.beginTransmission((uint8_t)LEDAddress);
  Wire.write(0x73);
  Wire.write(brightness);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (SetLEDBrightness(all))");
    return (false);
  }
  return (true);
}
boolean LEDOff(void) {
  Wire.beginTransmission((uint8_t)LEDAddress);
  Wire.write(0x74);
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (LEDOff)");
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
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack (ChangeAddress)");
    return (false);
  }
  return (true);
}

