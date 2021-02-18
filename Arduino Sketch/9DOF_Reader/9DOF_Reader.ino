
#include <Wire.h>
#include <cstdint>
#include "I2C_Transmit.h"

// I2C address when SD0 = VDDIO
uint8_t LISM6DS33_Address_Primary = 0x6A;
uint8_t LIS3MDL_Address_Primary = 0x1E;

bool rslt;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  uint8_t readData, *dPtr = &readData;

  rslt = I2C_Read_Data_Bytes(LIS3MDL_Address_Primary, 0x0F, dPtr, 1);
  if (!rslt)
    Serial.println("Error reading register ");
  Serial.printf("data WHO AM I REGISTER: %x", readData);

  rslt = I2C_Send_Data(LIS3MDL_Address_Primary, 0x21, 0x40);

  rslt = I2C_Send_Data(LIS3MDL_Address_Primary, 0x20, 0xFC);
  
  rslt = I2C_Send_Data(LIS3MDL_Address_Primary, 0x23, 0x0C);

  rslt = I2C_Send_Data(LIS3MDL_Address_Primary, 0x22, 0x00);
  

  /*I2C_Read_Data_Bytes(LISM6DS33_Address_Primary, 0x0F, dPtr, 1);

  Serial.printf("data WHO AM I REGISTER: %x", readData);

  rslt = I2C_Send_Data(LISM6DS33_Address_Primary, 0x18, 0x38);  // activate acc xyz axes
  if (!rslt)
    printf("Failed to activate accelerometer \n\r");

  rslt = I2C_Send_Data(LISM6DS33_Address_Primary, 0x10, 0x40); // accel activate ODR
  if (!rslt)
    printf("Failed to activate accelerometer \n\r");

  rslt = I2C_Send_Data(LISM6DS33_Address_Primary, 0x19, 0x38);  // activate gyro xyz axes
  if (!rslt)
    printf("Failed to activate accelerometer \n\r");

  rslt = I2C_Send_Data(LISM6DS33_Address_Primary, 0x11, 0x40); // gyro activate ODR
  if (!rslt)
    printf("Failed to activate gyroscopoe \n\r");*/

}

void loop() {

  uint8_t OUTmData[6];
  int16_t MAG_X;
  int16_t MAG_Y;
  int16_t MAG_Z;

  uint8_t regStatus = 0, *statPtr = &regStatus;
  
  rslt = I2C_Read_Data_Bytes(LIS3MDL_Address_Primary, 0x27, statPtr, 1);
  if (!rslt)
    Serial.print("Error reading status\n\r");
  regStatus >> 3;
  if (regStatus & 0x1 == 1) {

    rslt = I2C_Read_Data_Bytes(LIS3MDL_Address_Primary, 0x28, OUTmData, 6);
    if (!rslt)
      Serial.printf("Error reading data\n\r");

    MAG_X = (int16_t)OUTmData[1] << 8 | OUTmData[0];
    MAG_Y = (int16_t)OUTmData[3] << 8 | OUTmData[2];
    MAG_Z = (int16_t)OUTmData[5] << 8 | OUTmData[4];

    Serial.printf("x: %.2f y: %.2f z: %.2f\n\r", (float)MAG_X/6842, (float)MAG_Y/6842, (float)MAG_Z/6842);
    //Serial.printf("x: %d y: %d z: %d\n\r", MAG_X, MAG_Y, MAG_Z);
    
  }

  /*uint8_t OUTdata[12];
  uint8_t regStatus = 0, *statPtr = &regStatus;
  
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

  rslt = I2C_Read_Data_Bytes(LISM6DS33_Address_Primary, 0x1E, statPtr, 1);
  if (!rslt)
    printf("Failed to get status of data \n\r");
  //printf(" STATUS_REG is %x\n\r", regStatus);
  if (regStatus > 0)
  {

    rslt = I2C_Read_Data_Bytes(LISM6DS33_Address_Primary, 0x22, OUTdata, 12);
    if (!rslt)
      printf("Failed to get data \n\r");

    GYRO_X = (int16_t)OUTdata[1] << 8 | OUTdata[0];
    gOUT_X = (float)GYRO_X * 8.75 * 0.017453293 / 1000;
 
    GYRO_Y = (int16_t)OUTdata[3] << 8 | OUTdata[2];
    gOUT_Y = (float)GYRO_Y * 8.75 * 0.017453293 / 1000;

    GYRO_Z = (int16_t)OUTdata[5] << 8 | OUTdata[4];
    gOUT_Z = (float)GYRO_Z * 8.75 * 0.017453293 / 1000;
    

    ACCEL_X = (int16_t)OUTdata[7] << 8 | OUTdata[6];
    aOUT_X = (float)ACCEL_X * 0.061 / 1000;

    ACCEL_Y = (int16_t)OUTdata[9] << 8 | OUTdata[8];
    aOUT_Y = (float)ACCEL_Y * 0.061 / 1000;

    ACCEL_Z = (int16_t)OUTdata[11] << 8 | OUTdata[10];
    aOUT_Z = (float)ACCEL_Z * 0.061 / 1000;

    //Serial.printf("Gyro X: %d Y: %d Z: %d\t Accel X: %.2f Y: %.2f Z: %.2f\n\r", GYRO_X,GYRO_Y,GYRO_Z,aOUT_X,aOUT_Y,aOUT_Z);
    Serial.printf("X:%.2f Y: %.2f Z: %.2f\n\r",aOUT_X,aOUT_Y,aOUT_Z);
    Serial.printf("X:%.2f Y: %.2f Z: %.2f\n\r", gOUT_X, gOUT_Y, gOUT_Z);

  }*/

  delay(150);



}
