
/*
   LSM6DS33_AccelGyro_Sensor.h - Library for connecting and
   receiving data from LSM6DS33 magnetometer sensor,
   Created by Rokas Cesiunas, 22 FEB, 2021
*/

#ifndef LSM6DS33_ACCELGYRO_SENSOR_H
#define LSM6DS33_ACCELGYRO_SENSOR_H

#include "Arduino.h"  // Standard arduino functions
#include <Wire.h>     // I2C communication
#include <cstdint>    // C style integers

class LSM6DS33_AccelGyro
{
  public:
    LSM6DS33_AccelGyro(bool);
    uint8_t LSM6DS33_Init();
    uint8_t LSM6DS33_Config();
    uint8_t LSM6DS33_Get_Data(float*);


  private:
    uint8_t I2C_Address;                              // I2C Address to be used
    const uint8_t LSM6DS33_ADDRESS_PRIMARY = 0x6B;    // I2C address when SD0/SA0 pin = VDDIO
    const uint8_t LSM6DS33_ADDRESS_SECONDARY = 0x6A;  // I2C address when SD0/SA0 pin = GND

    // LSM6DS33 Register addresses
    const uint8_t LSM6DS33_WHO_AM_I = 0x0F;

    const uint8_t LSM6DS33_CTRL1_XL = 0x10;
    const uint8_t LSM6DS33_CTRL2_G = 0x11;
    const uint8_t LSM6DS33_CTRL3_C = 0x12;
    const uint8_t LSM6DS33_CTRL4_C = 0x13;
    const uint8_t LSM6DS33_CTRL5_C = 0x14;
    const uint8_t LSM6DS33_CTRL6_C = 0x15;
    const uint8_t LSM6DS33_CTRL7_C = 0x16;
    const uint8_t LSM6DS33_CTRL8_XL = 0x17;
    const uint8_t LSM6DS33_CTRL9_XL = 0x18;
    const uint8_t LSM6DS33_CTRL10_C = 0x19;

    const uint8_t LSM6DS33_STATUS_REG = 0x1E;

    const uint8_t LSM6DS33_OUT_TEMP_L = 0x20, LSM6DS33_OUT_TEMP_H = 0x21;


    //  LSM6DS33 CMD register commands
    const uint8_t LSM6DS33_SOFTRESET = 1;
    const uint8_t LSM6DS33_ENABLE_ALL_ACCEL_AXES = 0x38;
    const uint8_t LSM6DS33_80MS_ODR = 0x10;
    const uint8_t LSM6DS33_2G_ACCEL_FS = 0x0;
    const uint8_t LSM6DS33_ENABLE_ALL_GYRO_AXES = 0x38;
    const uint8_t LSM6DS33_250_DPS_GYRO_FS = 0x0;

    // LSM6DS33 Constants
    const uint8_t LSM6DS33_DATA_LENGHT = 12;
    const uint8_t LSM6DS33_DATA_START = 0x22;

    const uint8_t LSM6DS33_CHIP_ID = 0x69;

    const uint8_t NEW_ACCEL_DATA = 1 << 0;
    const uint8_t NEW_GYRO_DATA = 1 << 1;
    const uint8_t NEW_TEMP_DATA = 1 << 2;

    const float LINEAR_ACCEL_SENSITIVITY_2MG = 0.061;     // mg/LSB
    const float LINEAR_ACCEL_SENSITIVITY_4MG = 0.122;     // mg/LSB
    const float LINEAR_ACCEL_SENSITIVITY_8MG = 0.244;     // mg/LSB
    const float LINEAR_ACCEL_SENSITIVITY_16MG = 0.488;    // mg/LSB

    const float ANGULAR_RATE_SENSITIVITY_125MDPS = 4.375; // mdps/LSB
    const float ANGULAR_RATE_SENSITIVITY_250MDPS = 8.75;  // mdps/LSB
    const float ANGULAR_RATE_SENSITIVITY_500MDPS = 17.50; // mdps/LSB
    const float ANGULAR_RATE_SENSITIVITY_1000MDPS = 35;   // mdps/LSB
    const float ANGULAR_RATE_SENSITIVITY_2000MDPS = 70;   // mdps/LSB

    const float DEG_TO_RADS = 0.017453293;
};

#endif
