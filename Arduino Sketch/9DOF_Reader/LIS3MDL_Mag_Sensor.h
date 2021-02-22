
/*
   LIS3MDL_Mag_Sensor.h - Library for connecting and
   receiving data from LISMDL magnetometer sensor
   Created by Rokas Cesiunas 22 FEB, 2021
*/

#ifndef LIS3MDL_MAG_SENSOR_H
#define LIS3MDL_MAG_SENSOR_H

#include "Arduino.h"
#include <Wire.h>
#include <cstdint>

class LIS3MDL_Mag
{
  public:
    LIS3MDL_Mag(uint8_t);
    bool LIS3MDL_Init();
    bool LIS3MDL_Get_Data(float*);


  private:
    uint8_t I2C_Address;
    
    const uint8_t LIS3MDL_WHO_AM_I = 0x0F;
    const uint8_t LIS3MDL_CTRL_REG1 = 0x20;
    const uint8_t LIS3MDL_CTRL_REG2 = 0x21;
    const uint8_t LIS3MDL_CTRL_REG3 = 0x22;
    const uint8_t LIS3MDL_CTRL_REG4 = 0x23;
    const uint8_t LIS3MDL_CTRL_REG5 = 0x24;
    const uint8_t LIS3MDL_STATUS_REG = 0x27;
    const uint8_t LIS3MDL_MAG_DATA_START = 0x28;

    const uint8_t LIS3MDL_MAG_DATA_LENGHT = 6;



};

#endif
