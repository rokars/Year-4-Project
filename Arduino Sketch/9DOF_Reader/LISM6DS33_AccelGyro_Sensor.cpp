
/*
   LISM6DS33_AccelGyro_Sensor.h - Library for connecting and
   receiving data from LISM6DS33 magnetometer sensor
   Created by Rokas Cesiunas 22 FEB, 2021
*/

#include "Arduino.h"
#include "LISM6DS33_AccelGyro_Sensor.h"
#include "I2C_Transmit.h"
#include <Wire.h>
#include <cstdint>

/*
   Constructor
*/
LISM6DS33_AccelGyro::LISM6DS33_AccelGyro(uint8_t address) {
  I2C_Address = address;
}

bool LISM6DS33_AccelGyro::LISM6DS33_Init() {
  uint8_t readData = 0, *dPtr = &readData;
  bool rslt = false;
  
  I2C_Read_Data_Bytes(I2C_Address, 0x0F, dPtr, 1);

  Serial.printf("data WHO AM I REGISTER: %x", readData);

  rslt = I2C_Send_Data(I2C_Address, 0x18, 0x38);  // activate acc xyz axes
  if (!rslt)
    printf("Failed to activate accelerometer 1\n\r");
  delay(50);

  rslt = I2C_Send_Data(I2C_Address, 0x10, 0x10); // accel activate ODR
  if (!rslt)
    printf("Failed to activate accelerometer 2\n\r");
  delay(50);

  rslt = I2C_Send_Data(I2C_Address, 0x19, 0x38);  // activate gyro xyz axes
  if (!rslt)
    printf("Failed to activate accelerometer 3\n\r");
  delay(50);

  rslt = I2C_Send_Data(I2C_Address, 0x11, 0x10); // gyro activate ODR
  if (!rslt)
    printf("Failed to activate gyroscopoe 4\n\r");
  delay(50);
}

bool LISM6DS33_AccelGyro::LISM6DS33_Get_Data(float* sensData) {
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

  rslt = I2C_Read_Data_Bytes(I2C_Address, 0x1E, dPtr, 1);
  if (!rslt)
    printf("Failed to get status of data \n\r");
  //printf(" STATUS_REG is %x\n\r", regStatus);
  if (readData > 0)
  {

    rslt = I2C_Read_Data_Bytes(I2C_Address, 0x22, OUTdata, 12);
    if (!rslt)
      printf("Failed to get data \n\r");

    GYRO_X = (int16_t)OUTdata[1] << 8 | OUTdata[0];
    gOUT_X = (float)GYRO_X * 8.75 * 0.017453293 / 1000;
    *(sensData + 0) = gOUT_X;

    GYRO_Y = (int16_t)OUTdata[3] << 8 | OUTdata[2];
    gOUT_Y = (float)GYRO_Y * 8.75 * 0.017453293 / 1000;
    *(sensData + 1) = gOUT_Y;
    
    GYRO_Z = (int16_t)OUTdata[5] << 8 | OUTdata[4];
    gOUT_Z = (float)GYRO_Z * 8.75 * 0.017453293 / 1000;
    *(sensData + 2) = gOUT_Z;

    ACCEL_X = (int16_t)OUTdata[7] << 8 | OUTdata[6];
    aOUT_X = (float)ACCEL_X * 0.061 / 1000;
    *(sensData + 3) = aOUT_X;

    ACCEL_Y = (int16_t)OUTdata[9] << 8 | OUTdata[8];
    aOUT_Y = (float)ACCEL_Y * 0.061 / 1000;
    *(sensData + 4) = aOUT_Y;

    ACCEL_Z = (int16_t)OUTdata[11] << 8 | OUTdata[10];
    aOUT_Z = (float)ACCEL_Z * 0.061 / 1000;
    *(sensData + 5) = aOUT_Z;

    //Serial.printf("Gyro X: %d Y: %d Z: %d\t Accel X: %.2f Y: %.2f Z: %.2f\n\r", GYRO_X,GYRO_Y,GYRO_Z,aOUT_X,aOUT_Y,aOUT_Z);
    //Serial.printf("%.2f,%.2f,%.2f\n\r", aOUT_X, aOUT_Y, aOUT_Z);
    //Serial.printf("%.2f,%.2f,%.2f\n\r", gOUT_X, gOUT_Y, gOUT_Z);

  }
}
