
#include <Wire.h>
#include <cstdint>
#include <SoftwareSerial.h>
#include <WiFi.h>

#include "I2C_Transmit.h"
#include "BMP3XX_Sensor.h"
#include "LIS3MDL_Mag_Sensor.h"
#include "LSM6DS33_AccelGyro_Sensor.h"
#include "Function_Return_Codes.h"
#include "SIM28_GPS_Receiver.h"
#include "myWifiClient.h"

LIS3MDL_Mag mag(true);
LSM6DS33_AccelGyro accgyr(false);
BMP3XX_Sensor bmp(true);

uint8_t returnCode = 0;

const uint8_t a = 128;
unsigned char buffer[a];                   // buffer array for data receive over serial port
uint8_t count = 0;

SoftwareSerial SoftSerial(0, 2);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  bool ga = wifiClient_Init();
  if (!ga)
    Serial.println("*************** WIFI INIT FAIL *************");
  delay(100);

  returnCode = mag.LIS3MDL_Init();
  if (returnCode == 0) {
    returnCode = mag.LIS3MDL_Config();
    if (returnCode != 0)
      Serial.printf("Config LIS3MDL error %d\n\r", returnCode);
  }

  returnCode = accgyr.LSM6DS33_Init();
  if (returnCode == 0) {
    returnCode = accgyr.LSM6DS33_Config();
    if (returnCode != 0)
      Serial.printf("Config LSM6DS33 error %d\n\r", returnCode);
  }

  returnCode = bmp.BMP3XX_Board_Init();
  if (returnCode == 0) {
    returnCode = bmp.BMP388_Set_Options();
    if (returnCode != 0)
      Serial.printf("Config BMP388 error %d\n\r", returnCode);
  }
  else
    Serial.printf("comms error to bmp return %d\n\r", returnCode);
  delay(50);

  //bool gg = GPS_init(SoftSerial);
  //if (!gg)
  //  Serial.println("*************** GPS INIT FAIL****************");

  delay(100);

  Serial.println("MagX,MagY,MagZ,GyroX,GyroY,GyroZ,AccelX,AccelY,AccelZ,TempC,PressPa");
}

void loop() {

  float sensorData[11];

  returnCode = mag.LIS3MDL_Get_Data(sensorData);
  if (returnCode != 0)
    Serial.printf("Get Data LIS3MDL error %d\n\r", returnCode);


  returnCode = accgyr.LSM6DS33_Get_Data(sensorData + 3);
  if (returnCode != 0)
    Serial.printf("Get Data LSM6DS33 error %d\n\r", returnCode);

  bool rslt = false;

  rslt = bmp.BMP388_Get_Data(sensorData + 9);
  if (!rslt) {
    Serial.println(" Read Error ");
  }
  Serial.printf("\n\r\n\r%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%0.2f,%0.2f\n\r\n\r", sensorData[0], sensorData[1], sensorData[2], sensorData[3], sensorData[4], sensorData[5], sensorData[6], sensorData[7], sensorData[8], sensorData[9], sensorData[10]);

  //uint8_t gg = receiveGpsData(SoftSerial, buffer, a);
  //if (gg <= 0)
  //  Serial.println("*************** GPS READ FAIL****************");

  //Serial.write(buffer, gg);
  ////gg = 0;

  bool gh = wifiClient_PostReq();
  if (!gh)
    Serial.println("Wifi Err");

  //delay(1000);


}

/*
   void loop() {
  if (Serial.available()) {
   char c = Serial.read();
   Serial.write(c);
   mySerial.write(c);
  }
  if (mySerial.available()) {
    char c = mySerial.read();
    Serial.write(c);
  }
  }
*/

//Serial.printf("M:%.2f,%.2f,%.2f\n\r", sensorData[0], sensorData[1], sensorData[2]);//mag
//Serial.printf("G,%.2f,%.2f,%.2f\n\r", sensorData[3], sensorData[4], sensorData[5]);//gyro
//Serial.printf("A,%.2f,%.2f,%.2f\n\r", sensorData[6], sensorData[7], sensorData[8]);//accel
