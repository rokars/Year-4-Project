
/*
   SIM28_GPS_Receiver.cpp - Library for connecting and
   receiving data from SIM28 GPS Receiver,
   Created by Rokas Cesiunas, 11 MAR, 2021
*/

#include "Arduino.h"  // Standard arduino functions
#include "Function_Return_Codes.h"
#include "SIM28_GPS_Receiver.h"
#include <cstdint>    // C style integers
#include <SoftwareSerial.h>

/*
  Initialise GPS module
  input parameter is the SoftwareSerial object address
  starts communication at 9600 to send command to increase to 57600 (max that my module works at)
  test communication by listening for acknowledgement of test pmtk command
  set to GPRMC sentence only at 1hz 
 */
uint8_t GPS_init(SoftwareSerial &gpsSerial) {
  unsigned char ackBuf[20];
  int count = 5;

  gpsSerial.begin(9600);                 // the SoftSerial baud rate
  delay(250);

  gpsSerial.println(PMTK_SET_BAUD_57600);

  delay(20);
  gpsSerial.end();
  delay(20);
  gpsSerial.begin(57600);
  delay(20);

  gpsSerial.println(PMTK_TEST);

  delay(50);
  while (!gpsSerial.available()) {
    if (count-- == 0) {
      break;
    }
    delay(50);
  }


  if (receiveGpsData(gpsSerial, ackBuf, 20) > 0) {

    if (ackBuf[0] == '$' && ackBuf[1] == 'P' && ackBuf[2] == 'M' && ackBuf[3] == 'T' && ackBuf[4] == 'K' &&
        ackBuf[5] == '0' && ackBuf[6] == '0' && ackBuf[7] == '1' && ackBuf[8] == ',' && ackBuf[9] == '0' &&
        ackBuf[10] == ',' && ackBuf[11] == '3' && ackBuf[12] == '*' && ackBuf[13] == '3' && ackBuf[14] == '0') {
      Serial.println("Got ack");

      delay(100);
      gpsSerial.println(PMTK_SET_NMEA_OUTPUT_RMCONLY);
      delay(100);
      gpsSerial.println(PMTK_SET_NMEA_UPDATE_1HZ);

      return SENSOR_OK;
    }
    else
      return COMM_ERR;
  }
}

/*
  Query Software serial for any incoming data
  accepts SoftwareSerial object address, pointer to char array to store received data
  and size of this array
 */
uint8_t receiveGpsData(SoftwareSerial &gpsSerial, unsigned char* dataRec, uint8_t dataRecSize) {
  memset(dataRec, '\0', dataRecSize);
  uint8_t recCount = 0;

  if (gpsSerial.available()) {                    // if date is coming from software serial port ==> data is coming from SoftSerial shield
    while (gpsSerial.available()) {             // reading data into char array

      *(dataRec + recCount++) = gpsSerial.read();    // writing data into array
      if (recCount == dataRecSize)
        break;
    }
    return recCount;// if no data, transmission ends
  }
}
