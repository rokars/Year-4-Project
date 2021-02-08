/*

   Rokas Cesiunas
   01 Feb

*/

#include <SPI.h>
#include <SD.h>

const int chipSelPin = 5;
const int cardDetPin = 17;

bool jsonWriteHeader(char[], char[]);
bool jsonWriteData(char []);
bool jsonWriteFooter(char []);
bool cardInit(int, int);
File openSDFile(char [], char);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect, Needed for native USB port only
  }

  pinMode(cardDetPin, INPUT);

  cardInit();
  jsonWriteHeader("Kappa123", "testing");
  jsonWriteData("Kappa123");
  jsonWriteFooter("Kappa123");

  File myFileR;
  myFileR = openSDFile("Kappa123", 'R');
  while (myFileR.available()) {
    Serial.write(myFileR.read());
  }
  myFileR.close();
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

File openSDFile(char fileName[], char operation) {

  int fileNameMax = 8, fileExtwNull;
  char dataFileName[14] = {'/'};   //1 + 8 + 4 = 13 size + null = 14
  //char pre = '/';
  char post[] = ".txt";

  strncpy(&dataFileName[1], fileName, fileNameMax);   //strcat searches for null
  strncpy(&dataFileName[9], post, sizeof(dataFileName - fileExtwNull));

  File dataFile;
  if (cardDetect()) {
    switch (operation) {
      case 'W':
        dataFile = SD.open(dataFileName, FILE_WRITE);
        Serial.println("WRITING: ");
        break;
      case 'A':
        dataFile = SD.open(dataFileName, FILE_APPEND);
        Serial.println("APPENDING: ");
        break;
      default:
        dataFile = SD.open(dataFileName);
        Serial.println("READING: ");
    }

    if (!dataFile) {
      Serial.printf("Error opening file %s", dataFileName);
    } else {
      return dataFile;
    }
  }
}

bool jsonWriteHeader(char fileName[], char dataTitle[]) {

  File dataFile = openSDFile(fileName, 'W');

  dataFile.println("{");
  dataFile.printf("\t\"%s\": [", dataTitle);

  dataFile.close();

  return true;
}


bool jsonWriteData(char FileName[]) {

  File dataFile = openSDFile(FileName, 'A');

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
  
  return true;
}

bool jsonWriteFooter(char FileName[]) {

  File dataFile = openSDFile(FileName, 'A');

  dataFile.print("\t]\n\r}");
  dataFile.close();
  
  return true;
}
