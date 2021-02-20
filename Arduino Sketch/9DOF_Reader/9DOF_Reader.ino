
#include <Wire.h>
#include <cstdint>
#include "I2C_Transmit.h"

// I2C address when SD0 = VDDIO
uint8_t LISM6DS33_Address_Primary = 0x6A;
uint8_t LIS3MDL_Address_Primary = 0x1E;

const uint8_t LIS3MDL_WHO_AM_I = 0x0F;
const uint8_t LIS3MDL_CTRL_REG1 = 0x20;
const uint8_t LIS3MDL_CTRL_REG2 = 0x21;
const uint8_t LIS3MDL_CTRL_REG3 = 0x22;
const uint8_t LIS3MDL_CTRL_REG4 = 0x23;
const uint8_t LIS3MDL_CTRL_REG5 = 0x23;
const uint8_t LIS3MDL_STATUS_REG = 0x27;
const uint8_t LIS3MDL_MAG_DATA_START = 0x28;
const uint8_t LIS3MDL_MAG_DATA_LENGHT = 6;

int mydelay = 400;

bool rslt = false;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  uint8_t readData = 0, *dPtr = &readData;

  rslt = I2C_Read_Data_Bytes(LIS3MDL_Address_Primary, 0x0F, dPtr, 1);
  Serial.printf("device id = %x\n\r", readData);
  if (!rslt)
    Serial.println("Error reading register");
  if (readData != 0x3d) {
    Serial.println("Incorrect device ");
    while (1);
  }

  // x/y uh-performance, 10hz odr
  rslt = I2C_Send_Data(LIS3MDL_Address_Primary, LIS3MDL_CTRL_REG1, 0x70);
  if (!rslt)
    printf("error sending data 1\n\r");

  // +- 4 gauss full-scale config
  rslt = I2C_Send_Data(LIS3MDL_Address_Primary, LIS3MDL_CTRL_REG2, 0x00);
  if (!rslt)
    printf("error sending data 2\n\r");

  // block-data update
  rslt = I2C_Send_Data(LIS3MDL_Address_Primary, LIS3MDL_CTRL_REG5, 0x40);
  if (!rslt)
    printf("error sending data 4\n\r");

  // continous conversion //power down / idle
  rslt = I2C_Send_Data(LIS3MDL_Address_Primary, LIS3MDL_CTRL_REG3, 0x00);
  if (!rslt)
    printf("error sending data 5\n\r");

  // z uh-performance
  rslt = I2C_Send_Data(LIS3MDL_Address_Primary, LIS3MDL_CTRL_REG4, 0x0C);
  if (!rslt)
    printf("error sending data 3\n\r");






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

  uint8_t OUTmData[LIS3MDL_MAG_DATA_LENGHT] = { 0 };
  int16_t MAG_X;
  int16_t MAG_Y;
  int16_t MAG_Z;

  uint8_t regStatus = 0, *statPtr = &regStatus;

  rslt = I2C_Read_Data_Bytes(LIS3MDL_Address_Primary, LIS3MDL_STATUS_REG, statPtr, 1);
  if (!rslt)
    Serial.print("Error reading status\n\r");
  Serial.print("status : ");
  Serial.println(regStatus, BIN);

  if (regStatus & (0x01 << 3)  == 0x01 << 3) {

    rslt = I2C_Read_Data_Bytes(LIS3MDL_Address_Primary, LIS3MDL_MAG_DATA_START, OUTmData, LIS3MDL_MAG_DATA_LENGHT);
    if (!rslt)
      Serial.printf("Error reading data\n\r");

    for (int i = 0; i < LIS3MDL_MAG_DATA_LENGHT; i++) {
      Serial.print(OUTmData[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    MAG_X = (int16_t)(OUTmData[1] << 8 | OUTmData[0]);
    MAG_Y = (int16_t)(OUTmData[3] << 8 | OUTmData[2]);
    MAG_Z = (int16_t)(OUTmData[5] << 8 | OUTmData[4]);

    Serial.printf("%.2f,%.2f,%.2f\n\r", (float)MAG_X / 6842, (float)MAG_Y / 6842, (float)MAG_Z / 6842);
    //Serial.printf("x: %d y: %d z: %d\n\r", MAG_X, MAG_Y, MAG_Z);

  }

  else if (regStatus & (0x01 << 7) == 0x01 << 7) {
    Serial.println("***** OVERWRITTEN ***********");
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

  delay(100);



}
