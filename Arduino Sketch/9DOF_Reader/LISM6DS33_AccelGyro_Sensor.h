
/*
   LISM6DS33_AccelGyro_Sensor.h - Library for connecting and
   receiving data from LISM6DS33 magnetometer sensor
   Created by Rokas Cesiunas 22 FEB, 2021
*/

#ifndef LISM6DS33_ACCELGYRO_SENSOR_H
#define LISM6DS33_ACCELGYRO_SENSOR_H

#include "Arduino.h"
#include <Wire.h>
#include <cstdint>

class LISM6DS33_AccelGyro
{
  public:
    LISM6DS33_AccelGyro(uint8_t);
    bool LISM6DS33_Init();
    bool LISM6DS33_Get_Data(float*);


  private:
    uint8_t I2C_Address;
  
};

#endif
