
/*
   LIS3MDL_Mag_Sensor.h - Library for connecting and
   receiving data from LISMDL magnetometer sensor
   Created by Rokas Cesiunas 22 FEB, 2021
*/

#include "Arduino.h"
#include "LIS3MDL_Mag_Sensor.h"
#include "I2C_Transmit.h"
#include <Wire.h>
#include <cstdint>

/*
   Constructor
*/
LIS3MDL_Mag::LIS3MDL_Mag(uint8_t address) {
  I2C_Address = address;
}

bool LIS3MDL_Mag::LIS3MDL_Init() {
  uint8_t readData = 0, *dPtr = &readData;
  bool rslt = false;

  rslt = I2C_Read_Data_Bytes(I2C_Address, 0x0F, dPtr, 1);
  Serial.printf("device id = %x\n\r", readData);
  if (!rslt)
    Serial.println("Error reading register");
  if (readData != 0x3d) {
    Serial.println("Incorrect device ");
    while (1);
  }


  /*
     A typical wakeup sequence is summarized as follows:
     1. Write 40h in CTRL_REG2.   Sets full scale ±12 Hz.
     2. Write FCh in CTRL_REG1.  Sets UHP mode on the X/Y axes, ODR at 80 Hz and activates temperature sensor.
     3. Write 0Ch in CTRL_REG4.   Sets UHP mode on the Z-axis.
     4. Write 00h in CTRL_REG3.   Sets continuous-measurement mode.
  */


  // +- 4 gauss full-scale config
  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG2, 0x00);
  if (!rslt)
    printf("error sending data 2\n\r");
  delay(50);

  // x/y uh-performance, 10hz odr
  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG1, 0x70);
  if (!rslt)
    printf("error sending data 1\n\r");
  delay(50);

  // z uh-performance
  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG4, 0x0C);
  if (!rslt)
    printf("error sending data 3\n\r");
  delay(50);

  // block-data update
  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG5, 0x40);
  if (!rslt)
    printf("error sending data 4\n\r");
  delay(50);

  // continous conversion //power down / idle
  rslt = I2C_Send_Data(I2C_Address, LIS3MDL_CTRL_REG3, 0x00);
  if (!rslt)
    printf("error sending data 5\n\r");
}

bool LIS3MDL_Mag::LIS3MDL_Get_Data(float* sensData) {
  uint8_t OUTmData[LIS3MDL_MAG_DATA_LENGHT] = { 0 };
  int16_t MAG_X;
  int16_t MAG_Y;
  int16_t MAG_Z;

  uint8_t regStatus = 0, *statPtr = &regStatus;
  bool rslt = false;

  rslt = I2C_Read_Data_Bytes(I2C_Address, LIS3MDL_STATUS_REG, statPtr, 1);
  if (!rslt)
    Serial.print("Error reading status\n\r");
  //Serial.print("status : ");
  //Serial.println(regStatus, BIN);

  if (regStatus & (0x01 << 3)  == 0x01 << 3) {

    rslt = I2C_Read_Data_Bytes(I2C_Address, LIS3MDL_MAG_DATA_START, OUTmData, LIS3MDL_MAG_DATA_LENGHT);
    if (!rslt)
      Serial.printf("Error reading data\n\r");

    /*for (int i = 0; i < LIS3MDL_MAG_DATA_LENGHT; i++) {
      Serial.print(OUTmData[i], HEX);
      Serial.print(" ");
      }
      Serial.println();*/

    // order of operations
    MAG_X = (int16_t)OUTmData[1] << 8 | OUTmData[0];
    MAG_Y = (int16_t)OUTmData[3] << 8 | OUTmData[2];
    MAG_Z = (int16_t)OUTmData[5] << 8 | OUTmData[4];

    *sensData = (float)MAG_X / 6842;
    *(sensData + 1) = (float)MAG_Y / 6842;
    *(sensData + 2) = (float)MAG_Z / 6842;
  }

  else if (regStatus & (0x01 << 7) == 0x01 << 7) {
    Serial.println("***** OVERWRITTEN ***********");
  }
}
