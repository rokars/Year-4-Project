
/*
   LSM6DS33_AccelGyro_Sensor.h - Library for connecting and
   receiving data from LSM6DS33 magnetometer sensor,
   Created by Rokas Cesiunas, 22 FEB, 2021
*/

#include "Arduino.h"  // Standard arduino functions
#include "LSM6DS33_AccelGyro_Sensor.h"
#include "I2C_Transmit.h"
#include "Function_Return_Codes.h"
#include <Wire.h>     // I2C communication
#include <cstdint>    // C style integers

/*
   Constructor
   bool SD0 referring to i2c address jumper on board
   true = high, false = low
*/
LSM6DS33_AccelGyro::LSM6DS33_AccelGyro(bool SD0) {
  if (SD0)
    I2C_Address = LSM6DS33_ADDRESS_PRIMARY;
  else
    I2C_Address = LSM6DS33_ADDRESS_SECONDARY;
}

/*
   Initialise sensor
   Soft-reset and read device id to check if
   we are communicating with the correct chip
*/
uint8_t LSM6DS33_AccelGyro::LSM6DS33_Init() {

  uint8_t sensInfo = 0;
  uint8_t* sensInfoPtr = &sensInfo;
  bool rslt = false;

  rslt = I2C_Read_Data_Bytes(I2C_Address, LSM6DS33_WHO_AM_I, sensInfoPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  if (sensInfo != LSM6DS33_CHIP_ID)
    return DEV_ID_ERR;

  rslt = I2C_Send_Data(I2C_Address, LSM6DS33_CTRL3_C, LSM6DS33_SOFTRESET);
  if (!rslt)
    return COMM_ERR;

  return SENSOR_OK;
}

/*
   configure settings for reading data
*/
uint8_t LSM6DS33_AccelGyro::LSM6DS33_Config() {

  bool rslt = false;

  rslt = I2C_Send_Data(I2C_Address, LSM6DS33_CTRL9_XL, 0x38);  // activate acc xyz axes
  if (!rslt)
    return COMM_ERR;
  delay(10);

  rslt = I2C_Send_Data(I2C_Address, LSM6DS33_CTRL1_XL, 0x10); // accel activate ODR
  if (!rslt)
    return COMM_ERR;
  delay(10);

  rslt = I2C_Send_Data(I2C_Address, LSM6DS33_CTRL10_C, 0x38);  // activate gyro xyz axes
  if (!rslt)
    return COMM_ERR;
  delay(10);

  rslt = I2C_Send_Data(I2C_Address, LSM6DS33_CTRL2_G, 0x10); // gyro activate ODR
  if (!rslt)
    return COMM_ERR;

  return SENSOR_OK;
}

/*
   Read sensor data registers
   input param is pointer to float array
*/
uint8_t LSM6DS33_AccelGyro::LSM6DS33_Get_Data(float* sensData) {

  uint8_t statusReg = 0, agOutData[12];
  uint8_t* statPtr = &statusReg;
  bool rslt = false;

  int16_t gyro_X;
  int16_t gyro_Y;
  int16_t gyro_Z;

  int16_t accel_X;
  int16_t accel_Y;
  int16_t accel_Z;

  float aOUT_X;
  float aOUT_Y;
  float aOUT_Z;

  float gOUT_X;
  float gOUT_Y;
  float gOUT_Z;

  rslt = I2C_Read_Data_Bytes(I2C_Address, LSM6DS33_STATUS_REG, statPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
    
  if (statusReg & (NEW_ACCEL_DATA + NEW_GYRO_DATA + NEW_TEMP_DATA) == (NEW_ACCEL_DATA + NEW_GYRO_DATA + NEW_TEMP_DATA))
  {
    rslt = I2C_Read_Data_Bytes(I2C_Address, LSM6DS33_DATA_START, agOutData, LSM6DS33_DATA_LENGHT);
    if (!rslt)
      return READING_ERR;

    gyro_X = (int16_t)agOutData[1] << 8 | agOutData[0];
    gOUT_X = (float)gyro_X * ANGULAR_RATE_SENSITIVITY_250MDPS * DEG_TO_RADS / 1000;
    *(sensData + 0) = gOUT_X;

    gyro_Y = (int16_t)agOutData[3] << 8 | agOutData[2];
    gOUT_Y = (float)gyro_Y * ANGULAR_RATE_SENSITIVITY_250MDPS * DEG_TO_RADS / 1000;
    *(sensData + 1) = gOUT_Y;

    gyro_Z = (int16_t)agOutData[5] << 8 | agOutData[4];
    gOUT_Z = (float)gyro_Z * ANGULAR_RATE_SENSITIVITY_250MDPS * DEG_TO_RADS / 1000;
    *(sensData + 2) = gOUT_Z;

    accel_X = (int16_t)agOutData[7] << 8 | agOutData[6];
    aOUT_X = (float)accel_X * LINEAR_ACCEL_SENSITIVITY_2MG / 1000;
    *(sensData + 3) = aOUT_X;

    accel_Y = (int16_t)agOutData[9] << 8 | agOutData[8];
    aOUT_Y = (float)accel_Y * LINEAR_ACCEL_SENSITIVITY_2MG / 1000;
    *(sensData + 4) = aOUT_Y;

    accel_Z = (int16_t)agOutData[11] << 8 | agOutData[10];
    aOUT_Z = (float)accel_Z * LINEAR_ACCEL_SENSITIVITY_2MG / 1000;
    *(sensData + 5) = aOUT_Z;
  }
  return SENSOR_OK;
}
