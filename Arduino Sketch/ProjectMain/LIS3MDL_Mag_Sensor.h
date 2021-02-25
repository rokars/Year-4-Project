
/*
   LIS3MDL_Mag_Sensor.h - Library for connecting and
   receiving data from LISMDL magnetometer sensor
   Created by Rokas Cesiunas 22 FEB, 2021
*/

#ifndef LIS3MDL_MAG_SENSOR_H
#define LIS3MDL_MAG_SENSOR_H

#include "Arduino.h"  // Standard arduino functions
#include <Wire.h>     // I2C communication
#include <cstdint>    // C style integers

class LIS3MDL_Mag
{
  public:
    LIS3MDL_Mag(bool);
    uint8_t LIS3MDL_Init();
    uint8_t LIS3MDL_Config();
    uint8_t LIS3MDL_Get_Data(float*);


  private:
    uint8_t I2C_Address;                            // I2C Address to be used
    const uint8_t LIS3MDL_ADDRESS_PRIMARY = 0x1E;   // I2C address when SD0/SA1 pin = VDDIO
    const uint8_t LIS3MDL_ADDRESS_SECONDARY = 0x1C; // I2C address when SD0/SA1 pin = GND

    //  LIS3MDL Register addresses
    const uint8_t LIS3MDL_WHO_AM_I = 0x0F;
    const uint8_t LIS3MDL_CTRL_REG1 = 0x20;
    const uint8_t LIS3MDL_CTRL_REG2 = 0x21;
    const uint8_t LIS3MDL_CTRL_REG3 = 0x22;
    const uint8_t LIS3MDL_CTRL_REG4 = 0x23;
    const uint8_t LIS3MDL_CTRL_REG5 = 0x24;
    const uint8_t LIS3MDL_STATUS_REG = 0x27;
    
    const uint8_t LIS3MDL_MAG_DATA_START = 0x28;

    // LIS3MDL Constants
    const uint8_t LIS3MDL_MAG_DATA_LENGHT = 6;
    
    const uint8_t LIS3MDL_CHIP_ID = 0x3D;
    const uint8_t LIS3MDL_SOFTRESET = 0x04;

    const uint8_t NEW_MAG_DATA = 1 << 3;
    const uint8_t OVERRUN_MAG_DATA = 1 << 7;

    const uint16_t GAUSS_SENS_4 = 6842;    // LSB/gauss
    const uint16_t GAUSS_SENS_8 = 3421;    // LSB/gauss
    const uint16_t GAUSS_SENS_12 = 2281;   // LSB/gauss
    const uint16_t GAUSS_SENS_16 = 1711;   // LSB/gauss



};

#endif
