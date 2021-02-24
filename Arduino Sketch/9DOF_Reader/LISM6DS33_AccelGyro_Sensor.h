
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
    uint8_t LISM6DS33_Init();
    uint8_t LISM6DS33_Config();
    uint8_t LISM6DS33_Get_Data(float*);


  private:
    uint8_t I2C_Address;
    // I2C address when SD0/SA1 pin = VDDIO
    const uint8_t LISM6DS33_ADDRESS_PRIMARY = 0x6B;
    // I2C address when SD0/SA1 pin = GND
    const uint8_t LISM6DS33_ADDRESS_SECONDARY = 0x6A;

    const uint8_t LISM6DS33_WHO_AM_I = 0x0F;

    const uint8_t LISM6DS33_CTRL1_XL = 0x10;
    const uint8_t LISM6DS33_CTRL2_G = 0x11;  
    const uint8_t LISM6DS33_CTRL3_C = 0x12;
    const uint8_t LISM6DS33_CTRL4_C = 0x13;
    const uint8_t LISM6DS33_CTRL5_C = 0x14;
    const uint8_t LISM6DS33_CTRL6_C = 0x15;
    const uint8_t LISM6DS33_CTRL7_C = 0x16;
    const uint8_t LISM6DS33_CTRL8_XL = 0x17;
    const uint8_t LISM6DS33_CTRL9_XL = 0x18;
    const uint8_t LISM6DS33_CTRL10_C = 0x19;

    const uint8_t LISM6DS33_STATUS_REG = 0x1E;

    const uint8_t LISM6DS33_OUT_TEMP_L = 0x20, LISM6DS33_OUT_TEMP_H = 0x21;

    const uint8_t LISM6DS33_DATA_START = 0x22;
    const uint8_t LISM6DS33_DATA_LENGHT = 12;

    const uint8_t LISM6DS33_CHIP_ID = 0x69;

    const float LINEAR_ACCEL_SENSITIVITY_2MG = 0.061;
    const float LINEAR_ACCEL_SENSITIVITY_4MG = 0.122;
    const float LINEAR_ACCEL_SENSITIVITY_8MG = 0.244;
    const float LINEAR_ACCEL_SENSITIVITY_16MG = 0.488;

    const float ANGULAR_RATE_SENSITIVITY_125MDPS = 4.375;
    const float ANGULAR_RATE_SENSITIVITY_250MDPS = 8.75;
    const float ANGULAR_RATE_SENSITIVITY_500MDPS = 17.50;
    const float ANGULAR_RATE_SENSITIVITY_1000MDPS = 35;
    const float ANGULAR_RATE_SENSITIVITY_2000MDPS = 70;

    const float DEG_TO_RADS = 0.017453293;
};

#endif
