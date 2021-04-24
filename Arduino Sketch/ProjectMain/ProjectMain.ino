
#include <Wire.h>     // I2C communication
#include <cstdint>    // C style integers
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

uint8_t returnCode = SENSOR_OK;
bool runWifi = true;
bool runLIS3MDL = true;
bool runLSM6DS33 = true;
bool runBMP3XX = true;
bool runGPS = true;

const uint8_t gpsBufferSize = 128;
unsigned char gpsBuffer[gpsBufferSize];       // buffer array for data receive over SoftSerial port

SoftwareSerial SoftSerial(0, 2);

void setup() {

  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  if (runWifi) {
    returnCode = wifiClient_Init();
    if (returnCode != SENSOR_OK) {
      Serial.println("Initialisation WIFI error\n\r");
      runWifi = false;
    }
    delay(50);
  }

  if (runLIS3MDL) {
    returnCode = mag.LIS3MDL_Init();
    if (returnCode == SENSOR_OK) {
      returnCode = mag.LIS3MDL_Config();
      if (returnCode != SENSOR_OK)
        Serial.printf("Config LIS3MDL error %d\n\r", returnCode);
    }
    else {
      Serial.printf("Initialisation LIS3MDL error %d\n\r", returnCode);
      runLIS3MDL = false;
    }
    delay(50);
  }

  if (runLSM6DS33) {
    returnCode = accgyr.LSM6DS33_Init();
    if (returnCode == SENSOR_OK) {
      returnCode = accgyr.LSM6DS33_Config();
      if (returnCode != SENSOR_OK)
        Serial.printf("Config LSM6DS33 error %d\n\r", returnCode);
    }
    else {
      Serial.printf("Initialisation LSM6DS33 error %d\n\r", returnCode);
      runLSM6DS33 = false;
    }
    delay(50);
  }

  if (runBMP3XX) {
    returnCode = bmp.BMP3XX_Board_Init();
    if (returnCode == SENSOR_OK) {
      returnCode = bmp.BMP388_Set_Options();
      if (returnCode != SENSOR_OK)
        Serial.printf("Config BMP388 error %d\n\r", returnCode);
    }
    else {
      Serial.printf("Initialisation BMP388 error %d\n\r", returnCode);
      runBMP3XX = false;
    }
    delay(50);
  }

  if (runGPS) {
    returnCode = GPS_init(SoftSerial);
    if (returnCode != SENSOR_OK) {
      Serial.println("Initialisation GPS error\n\r");
      runGPS = false;
    }
    delay(50);
  }

  delay(100);
}

void loop() {

  float sensorData[11];

  if (runLIS3MDL) {
    returnCode = mag.LIS3MDL_Get_Data(sensorData);
    if (returnCode != SENSOR_OK)
      Serial.printf("Get Data LIS3MDL error %d\n\r", returnCode);
  }

  if (runLSM6DS33) {
    returnCode = accgyr.LSM6DS33_Get_Data(sensorData + 3);
    if (returnCode != SENSOR_OK)
      Serial.printf("Get Data LSM6DS33 error %d\n\r", returnCode);
  }

  if (runBMP3XX) {
    returnCode = bmp.BMP388_Get_Data(sensorData + 9);
    if (returnCode != SENSOR_OK)
      Serial.printf("Get Data BMP388 error %d\n\r", returnCode);
  }

  if (runGPS) {
    uint8_t recDataCount = receiveGpsData(SoftSerial, gpsBuffer, sizeof(gpsBuffer));
    if (recDataCount <= 0)
      Serial.println("GPS read Err");
  }

  if (runWifi) {
    returnCode = wifiClient_PostReq(sensorData, gpsBuffer);
    if (returnCode != SENSOR_OK)
      Serial.println("Wifi Err");
  }

  //  Serial.println("MagX,MagY,MagZ,GyroX,GyroY,GyroZ,AccelX,AccelY,AccelZ,TempC,PressPa");
  //  Serial.printf("\n\r\n\r%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%0.2f,%0.2f\n\r\n\r", sensorData[0], sensorData[1], sensorData[2], sensorData[3], sensorData[4], sensorData[5], sensorData[6], sensorData[7], sensorData[8], sensorData[9], sensorData[10]);

  delay(1000);
}
