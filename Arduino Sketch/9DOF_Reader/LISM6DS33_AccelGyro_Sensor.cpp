
/*
   LISM6DS33_AccelGyro_Sensor.h - Library for connecting and
   receiving data from LISM6DS33 magnetometer sensor
   Created by Rokas Cesiunas 22 FEB, 2021
*/

#include "Arduino.h"
#include "LISM6DS33_AccelGyro_Sensor.h"
#include "I2C_Transmit.h"
#include "Function_Return_Codes.h"
#include <Wire.h>
#include <cstdint>

/*
   Constructor
*/
LISM6DS33_AccelGyro::LISM6DS33_AccelGyro(uint8_t address) {
  I2C_Address = address;
}

/*
   Initialise sensor
   Soft-reset and read device id to check if
   we are communicating with the correct chip
*/
uint8_t LISM6DS33_AccelGyro::LISM6DS33_Init() {

  uint8_t sensInfo = 0;
  uint8_t* sensInfoPtr = &sensInfo;
  bool rslt = false;

  rslt = I2C_Read_Data_Bytes(I2C_Address, LISM6DS33_WHO_AM_I, sensInfoPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  if (sensInfo != LISM6DS33_CHIP_ID)
    return DEV_ID_ERR;

  rslt = I2C_Send_Data(I2C_Address, LISM6DS33_CTRL3_C, 0x01);
  if (!rslt)
    return COMM_ERR;

  return SENSOR_OK;

}

/*
   configure settings for reading data
*/
uint8_t LISM6DS33_AccelGyro::LISM6DS33_Config() {

  bool rslt = false;

  rslt = I2C_Send_Data(I2C_Address, LISM6DS33_CTRL9_XL, 0x38);  // activate acc xyz axes
  if (!rslt)
    printf("Failed to activate accelerometer 1\n\r");
  delay(50);

  rslt = I2C_Send_Data(I2C_Address, LISM6DS33_CTRL1_XL, 0x10); // accel activate ODR
  if (!rslt)
    printf("Failed to activate accelerometer 2\n\r");
  delay(50);

  rslt = I2C_Send_Data(I2C_Address, LISM6DS33_CTRL10_C, 0x38);  // activate gyro xyz axes
  if (!rslt)
    printf("Failed to activate accelerometer 3\n\r");
  delay(50);

  rslt = I2C_Send_Data(I2C_Address, LISM6DS33_CTRL2_G, 0x10); // gyro activate ODR
  if (!rslt)
    printf("Failed to activate gyroscopoe 4\n\r");
  delay(50);

  return SENSOR_OK;
}

/*
   Read sensor data registers
*/
uint8_t LISM6DS33_AccelGyro::LISM6DS33_Get_Data(float* sensData) {
  
  uint8_t readData = 0, *dPtr = &readData;
  uint8_t OUTdata[12];
  bool rslt = false;

  int16_t GYRO_X;
  int16_t GYRO_Y;
  int16_t GYRO_Z;

  int16_t ACCEL_X;
  int16_t ACCEL_Y;
  int16_t ACCEL_Z;

  float aOUT_X;
  float aOUT_Y;
  float aOUT_Z;

  float gOUT_X;
  float gOUT_Y;
  float gOUT_Z;

  rslt = I2C_Read_Data_Bytes(I2C_Address, LISM6DS33_STATUS_REG, dPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  if (readData > 0)
  {
    rslt = I2C_Read_Data_Bytes(I2C_Address, LISM6DS33_DATA_START, OUTdata, LISM6DS33_DATA_LENGHT);
    if (!rslt)
      return READING_ERR;
      
    GYRO_X = (int16_t)OUTdata[1] << 8 | OUTdata[0];
    gOUT_X = (float)GYRO_X * ANGULAR_RATE_SENSITIVITY_250MDPS * DEG_TO_RADS / 1000;
    *(sensData + 0) = gOUT_X;

    GYRO_Y = (int16_t)OUTdata[3] << 8 | OUTdata[2];
    gOUT_Y = (float)GYRO_Y * ANGULAR_RATE_SENSITIVITY_250MDPS * DEG_TO_RADS/ 1000;
    *(sensData + 1) = gOUT_Y;

    GYRO_Z = (int16_t)OUTdata[5] << 8 | OUTdata[4];
    gOUT_Z = (float)GYRO_Z * ANGULAR_RATE_SENSITIVITY_250MDPS * DEG_TO_RADS / 1000;
    *(sensData + 2) = gOUT_Z;

    ACCEL_X = (int16_t)OUTdata[7] << 8 | OUTdata[6];
    aOUT_X = (float)ACCEL_X * LINEAR_ACCEL_SENSITIVITY_2MG / 1000;
    *(sensData + 3) = aOUT_X;

    ACCEL_Y = (int16_t)OUTdata[9] << 8 | OUTdata[8];
    aOUT_Y = (float)ACCEL_Y * LINEAR_ACCEL_SENSITIVITY_2MG / 1000;
    *(sensData + 4) = aOUT_Y;

    ACCEL_Z = (int16_t)OUTdata[11] << 8 | OUTdata[10];
    aOUT_Z = (float)ACCEL_Z * LINEAR_ACCEL_SENSITIVITY_2MG / 1000;
    *(sensData + 5) = aOUT_Z;
  }
  return SENSOR_OK;
}
