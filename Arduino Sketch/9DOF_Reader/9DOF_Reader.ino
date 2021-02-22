
#include <Wire.h>
#include <cstdint>
#include "I2C_Transmit.h"
#include "LIS3MDL_Mag_Sensor.h"
#include "LISM6DS33_AccelGyro_Sensor.h"

// I2C address when SD0 = VDDIO
const uint8_t LISM6DS33_Address_Primary = 0x6A;
const uint8_t LIS3MDL_Address_Primary = 0x1E;

LIS3MDL_Mag mag(LIS3MDL_Address_Primary);
LISM6DS33_AccelGyro accgyr(LISM6DS33_Address_Primary);
bool rslt = false;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }


  rslt = mag.LIS3MDL_Init();
  rslt = accgyr.LISM6DS33_Init();

  if (!rslt) {
    Serial.println("error setting up sensors");
  }

}

void loop() {

  float sensorData[9];

  rslt = mag.LIS3MDL_Get_Data(sensorData);
  if (!rslt) {
    Serial.printf("error getting data\n\r");
  }

  //Serial.printf("Xm: %.2f, Ym: %.2f, Zm: %.2f\n\r", sensorData[0], sensorData[1], sensorData[2]);

  rslt = accgyr.LISM6DS33_Get_Data(sensorData + 3);
  if (!rslt) {
    Serial.printf("error getting data\n\r");
  }

  //Serial.printf("Xg: %.2f, Yg: %.2f, Zg: %.2f\n\r", sensorData[3], sensorData[4], sensorData[5]);
  //Serial.printf("Xa: %.2f, Ya: %.2f, Za: %.2f\n\r", sensorData[6], sensorData[7], sensorData[8]);

  
  Serial.printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n\r", sensorData[0], sensorData[1], sensorData[2], sensorData[3], sensorData[4], sensorData[5], sensorData[6], sensorData[7], sensorData[8]);
  //Serial.printf("G,%.2f,%.2f,%.2f\n\r", sensorData[3], sensorData[4], sensorData[5]);//gyro
  //Serial.printf("A,-%.2f,%.2f,%.2f\n\r", sensorData[6], sensorData[7], sensorData[8]);//accel

  delay(100);



}
