
#include <Wire.h>
#include <cstdint>
#include "I2C_Transmit.h"
#include "LIS3MDL_Mag_Sensor.h"
#include "LISM6DS33_AccelGyro_Sensor.h"
#include "Function_Return_Codes.h"

// I2C address when SD0 = VDDIO
const uint8_t LISM6DS33_Address_Primary = 0x6A;

LIS3MDL_Mag mag(true);
LISM6DS33_AccelGyro accgyr(LISM6DS33_Address_Primary);
uint8_t returnCode = 0;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }


  returnCode = mag.LIS3MDL_Init();
  if (returnCode != 0)
    Serial.printf("Init LIS3MDL error %d\n\r", returnCode);

  returnCode = mag.LIS3MDL_Config();
  if (returnCode != 0)
    Serial.printf("Config LIS3MDL error %d\n\r", returnCode);

  returnCode = accgyr.LISM6DS33_Init();
  if (returnCode != 0)
    Serial.printf("Init LISM6DS33 error %d\n\r", returnCode);

  returnCode = accgyr.LISM6DS33_Config();
  if (returnCode != 0)
    Serial.printf("Config LISM6DS33 error %d\n\r", returnCode);


}

void loop() {

  float sensorData[9];

  returnCode = mag.LIS3MDL_Get_Data(sensorData);
  if (returnCode != 0)
    Serial.printf("Get Data LIS3MDL error %d\n\r", returnCode);


  returnCode = accgyr.LISM6DS33_Get_Data(sensorData + 3);
  if (returnCode != 0)
    Serial.printf("Get Data LISM6DS33 error %d\n\r", returnCode);


  Serial.printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n\r", sensorData[0], sensorData[1], sensorData[2], sensorData[3], sensorData[4], sensorData[5], sensorData[6], sensorData[7], sensorData[8]);

  //Serial.printf("M:%.2f,%.2f,%.2f\n\r", sensorData[0], sensorData[1], sensorData[2]);//mag
  //Serial.printf("G,%.2f,%.2f,%.2f\n\r", sensorData[3], sensorData[4], sensorData[5]);//gyro
  //Serial.printf("A,%.2f,%.2f,%.2f\n\r", sensorData[6], sensorData[7], sensorData[8]);//accel

  delay(100);



}
