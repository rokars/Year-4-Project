
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

bool GPS_init(SoftwareSerial &gpsSerial) {

  gpsSerial.begin(9600);                 // the SoftSerial baud rate
  delay(250);

  gpsSerial.println(PMTK_SET_BAUD_57600);

  delay(20);
  gpsSerial.end();
  delay(20);
  gpsSerial.begin(57600);
  delay(20);

  gpsSerial.println(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  delay(100);
  gpsSerial.println(PMTK_SET_NMEA_UPDATE_1HZ);

  return true;
}

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

/*void clearBufferArray() {     // function to clear buffer array
  for (int i = 0; i < count; i++)
  {
    buffer[i] = NULL;     // clear all index of array with command NULL
  }
  }*/
