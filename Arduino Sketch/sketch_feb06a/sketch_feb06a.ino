
#include <Wire.h>
#include <cstdint>
#include "BMP3XX_Sensor.h"

// I2C address when SD0 = VDDIO
uint8_t BMP388_Address_Primary = 0x77;

uint32_t dataReceive1;
uint32_t dataReceive2;
long t_lin;
//long dataReceive3;
bool rslt;

float PAR_T1;
float PAR_T2;
float PAR_T3;
float PAR_P1;
float PAR_P2;
float PAR_P3;
float PAR_P4;
float PAR_P5;
float PAR_P6;
float PAR_P7;
float PAR_P8;
float PAR_P9;
float PAR_P10;
float PAR_P11;

bool read_Command();
bool send_Command();

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  //rslt = BMP3XX_Board_Init(BMP388_Address_Primary, BMP388_CALIB_DATA_START, BMP388_CALIB_DATA_LENGHT);

  //Serial.printf("\n\rResult: %d\n\r", rslt);

  //////////////////////////////////////////////////////

  /*Serial.print("\n\rReading register: ");
  Serial.print(BMP388_ERR_REG, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_ERR_REG, HEX);
  Serial.println(" hex");

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_ERR_REG));
  if (Wire.endTransmission())
    Serial.println(" ACK received ");

  Wire.requestFrom(byte(BMP388_Address_Primary), 1);
  Serial.print("Error Register reading: ");
  if (Wire.available() <= 1) {
    dataReceive1 = Wire.read();
  }
  Serial.println(dataReceive1, BIN);

  if (dataReceive1 >= 1 && dataReceive1 <= 7) {
    Serial.println("SENSOR ERROR");
  }
  delay(100);

  //////////////////////////////////////////////////////

  Serial.print("\n\rWriting to register: ");
  Serial.print(BMP388_PWR_CTRL, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_PWR_CTRL, HEX);
  Serial.println(" hex");
  Serial.print(" Value to be written: ");
  Serial.print(0b00110010, BIN);  //0x33
  Serial.print(" binary, ");
  Serial.print(0b00110010, HEX);
  Serial.println(" hex");

  Serial.print("\n\rWriting to register: ");
  Serial.print(BMP388_OSR, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_OSR, HEX);
  Serial.println(" hex");
  Serial.print(" Value to be written: ");
  Serial.print(0b00000011, BIN);  //0x03
  Serial.print(" binary, ");
  Serial.print(0b00000011, HEX);
  Serial.println(" hex");

  Serial.print("\n\rWriting to register: ");
  Serial.print(BMP388_CONFIG, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_CONFIG, HEX);
  Serial.println(" hex");
  Serial.print(" Value to be written: ");
  Serial.print(0b00000001, BIN);
  Serial.print(" binary, ");
  Serial.print(0b00000001, HEX);
  Serial.println(" hex");

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(BMP388_OSR);
  Wire.write(0b00000011);   //set over sampling: temp x1, press x8
  Wire.endTransmission(false);
  if (Wire.endTransmission())
    Serial.println(" ACK received ");
  delay(50);

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_PWR_CTRL));
  Wire.write(0b00110010);   //set power mode to normal, temp enabled
  Wire.endTransmission(false);
  if (Wire.endTransmission())
    Serial.println(" ACK received ");
  delay(50);

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_CONFIG));
  Wire.write(0b00000001);   //iir filter coeff 1
  Wire.endTransmission();
  if (Wire.endTransmission())
    Serial.println(" ACK received ");
  delay(50);

  //////////////////////////////////////////////////////

  Serial.print("\n\rReading registers: ");
  Serial.print(BMP388_PWR_CTRL, BIN);
  Serial.print(" (");
  Serial.print(BMP388_PWR_CTRL, HEX);
  Serial.println(") ");

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_PWR_CTRL));
  Wire.endTransmission(false);
  if (Wire.endTransmission())
    Serial.println(" ACK received ");

  Wire.requestFrom(BMP388_Address_Primary, 2);
  if (Wire.available() == 2) {
    dataReceive1 = Wire.read();
    dataReceive2 = Wire.read();
  }

  Serial.print("Power Ctrl register: ");
  Serial.print(dataReceive1, BIN);
  Serial.print(" OSR register: ");
  Serial.println(dataReceive2, BIN);

  Serial.println("\n\rReading temp data registers: ");
  */
  //////////////////////////////////////////////////////
}

void loop() {
  // put your main code here, to run repeatedly:
}

bool read_Command() {
  return true;
}
bool send_Command() {
  return true;
}

/*
  //////////////////////////////////////////////////////
  // Print I2C Address of which we are connnecting to //

  Serial.print("\n\rConnecting to slave with address: ");
  Serial.print(BMP388_Address_Primary, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_Address_Primary, HEX);
  Serial.println(" hex");
  delay(100);

  //////////////////////////////////////////////////////
  //          Print the CHIP_ID Register             //

  Serial.print("\n\rReading register: ");
  Serial.print(BMP388_CHIP_ID, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_CHIP_ID, HEX);
  Serial.println(" hex");

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_CHIP_ID));
  Wire.endTransmission();
  if (Wire.endTransmission())
    Serial.println(" ACK received ");

  Wire.requestFrom(byte(BMP388_Address_Primary), 1);
  Serial.print("Chip ID: ");
  if (Wire.available() == 1) {
    dataReceive1 = Wire.read();
  }
  Serial.print(dataReceive1, BIN);
  Serial.print(", in decimal: ");
  Serial.println(dataReceive1, DEC);
  delay(100);


*/
