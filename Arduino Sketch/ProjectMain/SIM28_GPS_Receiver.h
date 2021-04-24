
/*
   SIM28_GPS_Receiver.h - Library for connecting and
   receiving data from SIM28 GPS Receiver,
   Created by Rokas Cesiunas, 11 MAR, 2021
*/

#ifndef SIM28_GPS_RECEIVER_H
#define SIM28_GPS_RECEIVER_H

#include "Arduino.h"  // Standard arduino functions
#include <SoftwareSerial.h>

#define PMTK_TEST "$PMTK000*32\r\n"

#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_2HZ  "$PMTK220,500*2B"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"

/*
   Supported NMEA Sentences: 0 NMEA_SEN_GLL,
   // GPGLL interval - Geographic Position - Latitude longitude   1 NMEA_SEN_RMC,
   // GPRMC interval - Recomended Minimum Specific GNSS Sentence 2 NMEA_SEN_VTG,
   // GPVTG interval - Course Over Ground and Ground Speed   3 NMEA_SEN_GGA,
   // GPGGA interval - GPS Fix Data   4 NMEA_SEN_GSA,
   // GPGSA interval - GNSS DOPS and Active Satellites   5 NMEA_SEN_GSV,
   // GPGSV interval - GNSS Satellites in View   17 NMEA_SEN_ZDA,
   // GPZDA interval – Time & Date

   Supported Frequency Setting
   0 - Disabled or not supported sentence
   1 - Output once every one position fix
   2 - Output once every two position fixes
   3 - Output once every three position fixes
   4 - Output once every four position fixes
   5 - Output once every five position fixes
*/


/*
   RMC string example
   $GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598, ,*10
   Message ID,
   UTC Time hhmmss.sss,
   Status A = valid V = not valid,
   Latitude ddmm.mmmm,
   N/S indicator,
   Longitude dddmm.mmmm,
   E/W indicator,
   Speed over ground knots,
   Course over ground degrees,
   Date ddmmyy,
   Magnetic Variation degrees
   East/West indicator
   Mode
*/

// turn on only the second sentence (GPRMC)
const char PMTK_SET_NMEA_OUTPUT_RMCONLY[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29";
// turn on GPRMC and GGA
const char PMTK_SET_NMEA_OUTPUT_RMCGGA[] = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28";
// turn on ALL THE DATA
const char  PMTK_SET_NMEA_OUTPUT_ALLDATA[] = "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28";
// turn off output
const char PMTK_SET_NMEA_OUTPUT_OFF[] = "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28";
// restore system default
const char  PMTK_SET_DEFAULT[] = "$PMTK314,-1*04";

const char PMTK_SET_BAUD_115200[] = "$PMTK251,115200*1F"; ///< 115200 bps
const char PMTK_SET_BAUD_57600[] = "$PMTK251,57600*2C";   ///<  57600 bps
const char PMTK_SET_BAUD_9600[] = "$PMTK251,9600*17";     ///<   9600 bps

const char  PMTK_Q_RELEASE[] = "$PMTK605*31";

uint8_t GPS_init(SoftwareSerial &gpsSerial);

uint8_t receiveGpsData(SoftwareSerial &gpsSerial, unsigned char* dataRec, uint8_t dataRecSize);

#endif
