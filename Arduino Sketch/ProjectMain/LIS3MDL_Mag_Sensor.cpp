
/*
   LIS3MDL_Mag_Sensor.h - Library for connecting and
   receiving data from LISMDL magnetometer sensor,
   Created by Rokas Cesiunas, 22 FEB, 2021
*/

#include "Arduino.h" // Standard arduino functions
#include "LIS3MDL_Mag_Sensor.h"
#include "I2C_Transmit.h"
#include "Function_Return_Codes.h"
#include <Wire.h>     // I2C communication
#include <cstdint>    // C style integers

/*
   Constructor
   bool SD0 referring to I2C address jumper on board
   true = high, false = low
*/
LIS3MDL_Mag::LIS3MDL_Mag(bool SD0) {
  if (SD0)
    I2C_Address = LIS3MDL_ADDRESS_PRIMARY;
  else
    I2C_Address = LIS3MDL_ADDRESS_SECONDARY;
}

/*
   Initialise sensor
   Soft-reset and read device id to check if
   we are communicating with the correct chip
   Returns Function Return Code
*/
uint8_t LIS3MDL_Mag::LIS3MDL_Init() {

  uint8_t sensInfo = 0;
  uint8_t* sensInfoPtr = &sensInfo;
  bool rslt = false;

  rslt = I2C_Read_Data_Bytes(I2C_Address, LIS3MDL_WHO_AM_I, sensInfoPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  if (sensInfo != LIS3MDL_CHIP_ID)
    return DEV_ID_ERR;

  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG2, LIS3MDL_SOFTRESET);
  if (!rslt)
    return COMM_ERR;

  return SENSOR_OK;
}


/*
   configure settings for reading data
   Returns Function Return Code
*/
uint8_t LIS3MDL_Mag::LIS3MDL_Config() {

  bool rslt = false;

  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG2, LIS3MDL_4_GAUSS_FS);
  if (!rslt)
    return COMM_ERR;
  delay(10);

  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG1, LIS3MDL_TEMP_DISABLE | LIS3MDL_XY_UH_PERFORMANCE | LIS3MDL_800MS_ODR);
  if (!rslt)
    return COMM_ERR;
  delay(10);

  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG4, LIS3MDL_Z_UH_PERFORMANCE);
  if (!rslt)
    return COMM_ERR;
  delay(10);

  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG5, LIS3MDL_BLOCK_DATA_UPDATE);
  if (!rslt)
    return COMM_ERR;
  delay(10);

  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG3, LIS3MDL_CONTINUOUS_CONVERSION);
  if (!rslt)
    return COMM_ERR;

  return SENSOR_OK;
}


/*
   Read sensor data registers
   input parameter is pointer to float array
   Returns Function Return Code
*/
uint8_t LIS3MDL_Mag::LIS3MDL_Get_Data(float* sensData) {

  uint8_t mOutData[LIS3MDL_MAG_DATA_LENGHT] = { 0 };

  uint8_t statusReg = 0;
  uint8_t* statPtr = &statusReg;
  bool rslt = false;

  int16_t mag_X;
  int16_t mag_Y;
  int16_t mag_Z;

  rslt = I2C_Read_Data_Bytes(I2C_Address, LIS3MDL_STATUS_REG, statPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;

  if (statusReg & NEW_MAG_DATA  == NEW_MAG_DATA) {

    rslt = I2C_Read_Data_Bytes(I2C_Address, LIS3MDL_MAG_DATA_START, mOutData, LIS3MDL_MAG_DATA_LENGHT);
    if (!rslt)
      return READING_ERR;

    // order of operations = no brackets
    mag_X = (int16_t)mOutData[1] << 8 | mOutData[0];
    mag_Y = (int16_t)mOutData[3] << 8 | mOutData[2];
    mag_Z = (int16_t)mOutData[5] << 8 | mOutData[4];

    *(sensData + 0) = (float)mag_X / GAUSS_SENS_4;
    *(sensData + 1) = (float)mag_Y / GAUSS_SENS_4;
    *(sensData + 2) = (float)mag_Z / GAUSS_SENS_4;
  }

  else if (statusReg & OVERRUN_MAG_DATA == OVERRUN_MAG_DATA) {
    Serial.println("***** OVERWRITTEN ***********");
  }
  return SENSOR_OK;
}
