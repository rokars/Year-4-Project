
#include <Wire.h>
#include <cstdint>
#include <SoftwareSerial.h>

#include "I2C_Transmit.h"
#include "BMP3XX_Sensor.h"
#include "LIS3MDL_Mag_Sensor.h"
#include "LSM6DS33_AccelGyro_Sensor.h"
#include "Function_Return_Codes.h"

LIS3MDL_Mag mag(true);
LSM6DS33_AccelGyro accgyr(false);
BMP3XX_Sensor bmp(true);

SoftwareSerial SoftSerial(0, 2);
const int a = 64;
unsigned char buffer[a];                   // buffer array for data receive over serial port
int count = 0;

uint8_t returnCode = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  SoftSerial.begin(9600);                 // the SoftSerial baud rate
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

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
  //Serial.printf("\n\r\n\r%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%0.2f,%0.2f\n\r\n\r", sensorData[0], sensorData[1], sensorData[2], sensorData[3], sensorData[4], sensorData[5], sensorData[6], sensorData[7], sensorData[8], sensorData[9], sensorData[10]);
  
  receiveGpsData(buffer, a);
  Serial.write(buffer, count);
  Serial.printf("count = %d\n\r\n\r", count);

  delay(100);
}

void receiveGpsData(unsigned char* dataRec, uint8_t dataRecSize) {
  memset(dataRec, '\0', dataRecSize);
  count = 0;

  if (SoftSerial.available()) {                    // if date is coming from software serial port ==> data is coming from SoftSerial shield
    while (SoftSerial.available()) {             // reading data into char array

      *(dataRec + count++) = SoftSerial.read();    // writing data into array
      if (count == a)
        break;
    }
  }
  if (Serial.available())                 // if data is available on hardware serial port ==> data is coming from PC or notebook
    SoftSerial.write(Serial.read());        // write it to the SoftSerial shield
}

/*void clearBufferArray() {     // function to clear buffer array
  for (int i = 0; i < count; i++)
  {
    buffer[i] = NULL;     // clear all index of array with command NULL
  }
  }*/

//Serial.printf("M:%.2f,%.2f,%.2f\n\r", sensorData[0], sensorData[1], sensorData[2]);//mag
//Serial.printf("G,%.2f,%.2f,%.2f\n\r", sensorData[3], sensorData[4], sensorData[5]);//gyro
//Serial.printf("A,%.2f,%.2f,%.2f\n\r", sensorData[6], sensorData[7], sensorData[8]);//accel
