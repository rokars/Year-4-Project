
/*
  BMP3XX_Sensor.h - Library for connecting and receiving data from
  Bosch BMP3XX family sensors,
  Created by Rokas Cesiunas, 09 FEB 2021.
*/

#include "Arduino.h"
#include "BMP3XX_Sensor.h"
#include "I2C_Transmit.h"
#include <Wire.h>
#include <cstdint>

// constructor
BMP3XX_Sensor::BMP3XX_Sensor(uint8_t address) {
  I2C_Address = address;
}

bool BMP3XX_Sensor::BMP3XX_Board_Init() {

  //  Soft reset sensor to erase any possible old settings
  I2C_Send_Data(I2C_Address, BMP388_CMD, BMP388_softreset);
  delay(5);
  // TO-DO check por register and cmd ready register

  uint8_t dataIn[1];
  // Read chip id number to ensure good communication
  dataIn[0] = I2C_Read_ByteData(I2C_Address, BMP388_CHIP_ID);
  if (dataIn[0] != BMP388_CHIP_ID_NO) {
    Serial.printf("\n\rRead incorrect Chip ID 0x%x stopping\n\r", dataIn);
    return false;
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
  BMP3XX_Trim_Data_Verify(trim_data);
  return true;
}

bool BMP3XX_Sensor::BMP388_Command_Ready() {

  /*uint8_t STATUS;

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
  */
}

void BMP3XX_Sensor::BMP3XX_Trim_Data_Verify(uint8_t arr[]) {

  for (byte i = 0; i < BMP388_CALIB_DATA_LENGHT; i++) {
    Serial.printf("NVM_TRIM_DATA[%d]: 0x%x [ ", i, arr[i]);
    Serial.print(arr[i], BIN);
    Serial.printf(" ]\n\r");
    }
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
