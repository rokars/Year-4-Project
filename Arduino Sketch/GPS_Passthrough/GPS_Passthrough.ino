#include <SoftwareSerial.h>

SoftwareSerial SoftSerial(0, 2);            // rx, tx
const int a = 128;
unsigned char buffer[a];                   // buffer array for data receive over serial port
char ackRec[] = "$PMTK001,0,3*30";

int count = 0;                              // counter for buffer array

//void gpsInit(SoftwareSerial&);
//void gpsReceive(SoftwareSerial&, unsigned char* , uint8_t);

void setup()
{

  Serial.begin(115200);                     // the Serial port of Arduino baud rate.

  gpsInit(SoftSerial);

}

void loop()
{
  gpsReceive(SoftSerial, buffer, a);

  /*if (count > 0) {
    for (int i = 0; i < count; i++) {
      Serial.print(buffer[i]);
    }
    Serial.println();

    Serial.printf("Count %d\n\r", count);
    count = 0;
    }*/
}

/*void gpsReceive(SoftwareSerial &sftsrl, unsigned char *buff, uint8_t len) {
  memset(buff, '\0', len);
  uint8_t c = 0;

  if (sftsrl.available()) {                    // if data is coming from software serial port ==> data is coming from SoftSerial shield
    while (sftsrl.available()) {             // reading data into char array
      //buff[count++] = sftsrl.read();    // writing data into array
      (buff + c++) = sftsrl.read();    // writing data into array
      if (c == len)break;
    }
    Serial.write(buff, c);                // if no data, transmission ends, write buffer to hardware serial port
    //memset(buff, '\0', len);                      // call clearBufferArray function to clear the stored data from the array
    //count = 0;                                  // set counter of while loop to zero
  }
  }*/

uint8_t gpsReceive(SoftwareSerial &gpsSerial, unsigned char* dataRec, uint8_t dataRecSize) {
  memset(dataRec, '\0', dataRecSize);
  uint8_t recCount = 0;

  if (gpsSerial.available()) {                    // if date is coming from software serial port ==> data is coming from SoftSerial shield
    while (gpsSerial.available()) {             // reading data into char array

      *(dataRec + recCount++) = gpsSerial.read();    // writing data into array
      if (recCount == dataRecSize)
        break;
    }
    Serial.write(dataRec, recCount);                // if no data, transmission ends, write buffer to hardware serial port
  }
}

/*void clearBufferArray()                     // function to clear buffer array
  {
  for (int i = 0; i < count; i++)
  {
    buffer[i] = NULL;
  }                      // clear all index of array with command NULL
  }*/

void gpsInit(SoftwareSerial &sftsrl) {

  unsigned char ackBuf[20];
  
  sftsrl.begin(9600);                 // the SoftSerial baud rate
  delay(250);

  Serial.println("Software Serial GPS Test Echo Test");
  // you can send various commands to get it started
  //mySerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  sftsrl.println("$PMTK251,57600*2C");

  delay(20);
  sftsrl.end();
  delay(20);
  sftsrl.begin(57600);
  delay(20);

  Serial.println("\n\rsending packet");
  sftsrl.println("$PMTK000*32");

  delay(50);
  while (!sftsrl.available())
  {}

  Serial.write(ackBuf, 20);
  Serial.println("received");

  if(ackBuf[0] == '$' && ackBuf[1] == 'P' && ackBuf[2] == 'M' && ackBuf[3] == 'T' && ackBuf[4] == 'K' && ackBuf[5] == '0' && ackBuf[6] == '0' && ackBuf[7] == '1'
     && ackBuf[8] == ',' && ackBuf[9] == '0' && ackBuf[10] == ',' && ackBuf[11] == '3' && ackBuf[12] == '*' && ackBuf[13] == '3' && ackBuf[14] == '0') {
    Serial.println("GOt ack ");
  }
  else
    Serial.println("not got");

  delay(100);
  sftsrl.println("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29");
  delay(100);
  sftsrl.println("$PMTK220,500*2B");
  delay(100);

}

/*if (Serial.available()) {       // if data is available on hardware serial port ==> data is coming from PC or notebook
  unsigned char e[50];
  int i = 0;
  while (Serial.available()) {
    e[i++] = Serial.read();
  }
  Serial.println("\n\rSent");

  Serial.write(e, i);

  Serial.println();

  sftsrl.write(e, i);
  }*/

//if (Serial.available())                 // if data is available on hardware serial port ==> data is coming from PC or notebook
//  SoftSerial.write(Serial.read());        // write it to the SoftSerial shield
