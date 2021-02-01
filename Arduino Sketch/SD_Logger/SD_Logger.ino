/*

   Rokas Cesiunas
   01 Feb

*/

#include <SPI.h>
#include <SD.h>
#include <string.h>

const int chipSelPin = 5;
const int cardDetPin = 17;

bool jsonWriteHeader(char[], char[]);
bool jsonWriteData(char []);
bool jsonWriteFooter(char []);
bool cardInit(int, int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect, Needed for native USB port only
  }

  pinMode(cardDetPin, INPUT);

  cardInit();

  if (jsonWriteHeader("HelloFile", "FebTest")) {
    File myFileR;
    myFileR = SD.open("/HelloFil.txt");
    if (myFileR) {
      Serial.println("Reading: ");
    }
    while (myFileR.available()) {
      Serial.write(myFileR.read());
    }
    myFileR.close();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

bool cardDetect() {
  if (digitalRead(cardDetPin))
    return true;
  else {
    Serial.print("No SD card inserted");
    delay(5000);
    if (digitalRead(cardDetPin))
      return true;
    else
      return false;
  }
}

bool cardInit() {

  if (cardDetect()) {
    Serial.println("Initializing SD card...");
    if (!SD.begin(chipSelPin)) {
      Serial.println("Card failed");
      while (1);
    }
    Serial.println("Card initialized");
  }
}

bool jsonWriteHeader(char fileName[], char dataTitle[]) {

  File dataFile;
  char dataFileName[14] = {'/'};   //1 + 8 + 4 = 13 size + null = 14
  //char pre = '/';
  char post[] = ".txt";

  strncat(dataFileName, fileName, 8);
  strcat(dataFileName, post);

  if (cardDetect()) {
    dataFile = SD.open(dataFileName, FILE_WRITE);
    if (!dataFile) {
      Serial.printf("Error opening file %s", dataFileName);
    } else {
      dataFile.println("{");
      dataFile.printf("\t\"%s\": [", dataTitle);
  
      dataFile.close();
  
      jsonWriteData(dataFileName);
  
      return true;
    }
  }
}

bool jsonWriteData(char FileName[]) {

  File dataFile = SD.open(FileName, FILE_APPEND);
  if (!dataFile) {
    Serial.println("Error opening file");
  } else {
    for (int i = 0; i < 5; i++) {
      if (i == 0) {
        dataFile.println("{");
      } else
        dataFile.println("\t\t{");

      dataFile.println("\t\t\t\"TimeStampDate\": \"2021-01-31T14:22:01Z\",");
      dataFile.println("\t\t\t\"AltitudeQnh_meters\": 15,");
      dataFile.println("\t\t\t\"AirSpeed_knots\": 5,");
      dataFile.println("\t\t\t\"VerticalSpeed_ms\": 1.2,");
      dataFile.println("\t\t\t\"NormalAcceleration_g\": 2.4,");
      dataFile.println("\t\t\t\"MotorPower_volts\": 7.4");

      if (i < 4) {
        dataFile.println("\t\t},");
      } else
        dataFile.println("\t\t}");
    }
    dataFile.close();

    jsonWriteFooter(FileName);

    return true;
  }
}

bool jsonWriteFooter(char FileName[]) {

  File dataFile = SD.open(FileName, FILE_APPEND);
  if (!dataFile) {
    Serial.printf("Error opening file");
  } else {
    dataFile.print("\t]\n\r}");
    dataFile.close();
  }
  return true;
}
