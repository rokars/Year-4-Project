
/*
  BMP3XX_Sensor.h - Library for connecting and receiving data from
  Bosch BMP3XX family sensors,
  Created by Rokas Cesiunas, 09 FEB 2021.
*/

#include "Arduino.h"
#include "BMP3XX_Sensor.h"
#include <Wire.h>
#include <cstdint>

// constructor
BMP3XX_Sensor::BMP3XX_Sensor(uint8_t address) {
  I2C_Address = address;
}

bool BMP3XX_Sensor::BMP3XX_Board_Init() {

  //  Soft reset sensor to erase any possible old settings
  Wire.beginTransmission(I2C_Address);
  Wire.write(byte(BMP388_CMD));
  Wire.write(byte(BMP388_softreset));
  if (Wire.endTransmission() > 0) {
    Serial.printf("\n\r *** Error Connecting to 0x%x *** \n\r", I2C_Address);
    return false;
  }
  else {
    Serial.printf("\n\rPerformed softreset on BMP sensor 0x%x \n\r", I2C_Address);
    delay(5);
  }

  uint8_t dataIn;

  // Read chip id number to ensure good communication
  Wire.beginTransmission(I2C_Address);
  Wire.write(byte(BMP388_CHIP_ID));
  if (Wire.endTransmission(false) > 0) { // do not send stop messsage
    Serial.println("\n\r *** CHIP ID Read fail no ack \n\r");
    return false;
  }
  else {
    Wire.requestFrom(I2C_Address, (uint8_t)1);
    if (Wire.available() == 1) {
      dataIn = Wire.read();
      if (dataIn != BMP388_CHIP_ID_NO) {
        Serial.printf("\n\rRead incorrect Chip ID 0x%x stopping\n\r", dataIn);
        return false;
      }
    }
  }

  //  Read Trimming Coefficients from Register   //
  Serial.println("\n\rReading Trimming Coefficients ");

  Wire.beginTransmission(I2C_Address);
  Wire.write(byte(BMP388_CALIB_DATA_START));
  if (Wire.endTransmission(false) > 0) {
    Serial.println("\n\r no ack ");
    return false;
  }

  uint8_t trim_data[BMP388_CALIB_DATA_LENGHT];

  Wire.requestFrom(I2C_Address, BMP388_CALIB_DATA_LENGHT);
  if (Wire.available() == BMP388_CALIB_DATA_LENGHT) {
    for (byte i = 0; i < BMP388_CALIB_DATA_LENGHT; i++) {
      trim_data[i] = Wire.read();
    }
  }
  BMP3XX_Trim_Data_Verify();
  return true;
}

bool BMP3XX_Sensor::BMP388_Command_Ready() {

  uint8_t STATUS;

  Wire.begin(I2C_Address);
  Wire.write(byte(0x03));
  Wire.endTransmission();

  Wire.requestFrom(byte(I2C_Address), (uint8_t)1);
  if (Wire.available() == 1) {
    STATUS = Wire.read();
  }
  STATUS = STATUS >> 4;
  if ( STATUS & 0b0001 == 1) {
    return true;
  }
  else
    return false;
}

void BMP3XX_Sensor::BMP3XX_Trim_Data_Verify() {

  /*for (byte i = 0; i < BMP388_CALIB_DATA_LEN; i++) {
    Serial.printf("NVM_TRIM_DATA[%d]: 0x%x [ ", i, trimming_data[i]);
    Serial.print(trimming_data[i], BIN);
    Serial.printf(" ]\n\r");
    }*/
}

void BMP3XX_Sensor::BMP388_Compensate_Temperature(uint32_t uncomp_temp) {
  /*float partial_data1;
    float partial_data2;

    partial_data1 = (float)(uncomp_temp - PAR_T1);
    partial_data2 = (float)(partial_data1 * PAR_T2);

    t_lin = partial_data2 + (partial_data1 * partial_data1) * PAR_T3;

    Serial.print("t_lin compensated data?: ");
    //Serial.print(t_lin, BIN);
    //Serial.print(" binary, ");
    //Serial.print(t_lin, HEX);
    Serial.print(" decimal: ");
    Serial.println(t_lin);*/

}
