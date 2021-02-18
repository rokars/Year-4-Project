
/*
  I2C_Transmit.h - Library for sending and receiving
  data through I2C protocol,
  Created by Rokas Cesiunas, 10 FEB 2021.
*/

#include "Arduino.h"
#include "I2C_Transmit.h"
#include <Wire.h>
#include <cstdint>


bool I2C_Send_Data(uint8_t add, uint8_t reg, uint8_t dat) {
  Wire.beginTransmission(byte(add));
  Wire.write(byte(reg));
  Wire.write(byte(dat));
  if (Wire.endTransmission() == 0)
    return true;
  else 
    return false;
}

bool I2C_Read_Data_Bytes(uint8_t add, uint8_t reg, uint8_t* dat, uint8_t len) {

  Wire.beginTransmission(byte(add));
  Wire.write(byte(reg));
  if (Wire.endTransmission(false) == 0) {  // do not send stop messsage
    Wire.requestFrom(add, (uint8_t)len);
    if (Wire.available() == len) {
      for (int i = 0; i < len; i++) {
        *(dat + i) = Wire.read();
      }
      return true;
    }
  }
  else {
    Serial.printf("\n\r *** Read 0x%x, register 0x%x failed ***\n\r", add, reg);
    return false;
  }
}
