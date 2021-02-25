
/*
  I2C_Transmit.h - Library for sending and receiving
  data through I2C protocol,
  Created by Rokas Cesiunas, 10 FEB 2021.
*/

#ifndef I2C_TRANSMIT_H
#define I2C_TRANSMIT_H

#include "Arduino.h"  // Standard arduino functions
#include <Wire.h>     // I2C communication
#include <cstdint>    // C style integers

bool I2C_Send_Data(uint8_t add, uint8_t reg, uint8_t dat);

bool I2C_Read_Data_Bytes(uint8_t add, uint8_t reg, uint8_t* dat, uint8_t len);

const uint8_t ONE_BYTE = 1;

#endif
