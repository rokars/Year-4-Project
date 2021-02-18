
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

/*
   Constructor
*/
BMP3XX_Sensor::BMP3XX_Sensor(uint8_t address) {
  I2C_Address = address;
}

/*
   Self-test
   Soft reset -> read chip ID -> read trimming data
   Returns true code if tests pass
*/
bool BMP3XX_Sensor::BMP3XX_Board_Init() {

  uint8_t sensorInfo = 0;
  uint8_t* sensorInfoPtr = &sensorInfo;
  bool rslt = false;

  //  Soft reset sensor to erase any possible old settings
  rslt = I2C_Send_Data(I2C_Address, BMP388_CMD, BMP388_softreset);
  if (!rslt)
    return false;

  //  Delay to give time to reset
  uint8_t count = 5;
  do {
    delay(5);
    count--;
    if (count == 0)
      return false;
  } while (!BMP388_Command_Ready());

  //  Query Event register for power on reset complete (por_detected)
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_EVENT, sensorInfoPtr, 1);
  if (!rslt || !sensorInfo)
    return false;

  // Read chip ID number to ensure good communication
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_CHIP_ID, sensorInfoPtr, 1);
  if (!rslt || sensorInfo != BMP388_CHIP_ID_NO)
    return false;

  //  Read Trimming Coefficients from Register
  uint8_t trim_data[BMP388_CALIB_DATA_LENGHT_BYTES];
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_CALIB_DATA_START, trim_data, BMP388_CALIB_DATA_LENGHT_BYTES);
  if (!rslt)
    return false;

  BMP3XX_Trim_Data_Parse(trim_data);

  //  set to forced mode for one measurement
  I2C_Send_Data(I2C_Address, BMP388_PWR_CTRL, 0x13);
  delay(50);

  uint8_t sensorTestData[BMP388_DATA_LENGHT_BYTES];
  uint32_t uTestTemp, uTestPress, data_msb, data_lsb, data_xlsb;

  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_DATA_0, sensorTestData, BMP388_DATA_LENGHT_BYTES);
  if (!rslt)
    return false;

  data_xlsb = sensorTestData[0];
  data_lsb = sensorTestData[1] << 8;
  data_msb = sensorTestData[2] << 16;
  uTestPress = data_msb | data_lsb | data_xlsb;

  data_xlsb = sensorTestData[3];
  data_lsb = sensorTestData[4] << 8;
  data_msb = sensorTestData[5] << 16;
  uTestTemp = data_msb | data_lsb | data_xlsb;

  float cTestTemp = BMP388_Compensate_Temperature(uTestTemp);
  float cTestPress = BMP388_Compensate_Pressure(uTestPress, cTestTemp);

  if (cTestTemp <= TEMP_UPPER_LIM_C && cTestTemp >= TEMP_LOWER_LIM_C) {
    if (cTestPress <= PRESS_UPPER_LIM_HPA && cTestPress >= PRESS_LOWER_LIM_HPA) {
      return false;
    }
  }

  return true;
}

/*

*/
bool BMP3XX_Sensor::BMP388_Command_Ready() {

  uint8_t rslt = 0, sensorStatus, *stPtr = &sensorStatus;
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_STATUS, stPtr, 1);
  if (rslt) {
    sensorStatus = sensorStatus >> 4;
    if (sensorStatus & 0b0001 == 1)
      return true;
    else
      return false;
  }
}

/*

*/
void BMP3XX_Sensor::BMP3XX_Trim_Data_Parse(uint8_t *trim_arr) {

  NVM_PAR_T1 = (uint16_t) * (trim_arr + 1) << 8 | (uint16_t) * trim_arr;
  NVM_PAR_T2 = (uint16_t) * (trim_arr + 3) << 8 | (uint16_t) * (trim_arr + 2);
  NVM_PAR_T3 = *(trim_arr + 4);
  NVM_PAR_P1 = (int16_t) * (trim_arr + 6) << 8 | (int16_t) * (trim_arr + 5);
  NVM_PAR_P2 = (int16_t) * (trim_arr + 8) << 8 | (int16_t) * (trim_arr + 7);
  NVM_PAR_P3 = *(trim_arr + 9);
  NVM_PAR_P4 = *(trim_arr + 10);
  NVM_PAR_P5 = (uint16_t) * (trim_arr + 12) << 8 | (uint16_t) * (trim_arr + 11);
  NVM_PAR_P6 = (uint16_t) * (trim_arr + 14) << 8 | (uint16_t) * (trim_arr + 13);
  NVM_PAR_P7 = *(trim_arr + 15);
  NVM_PAR_P8 = *(trim_arr + 16);
  NVM_PAR_P9 = (int16_t) * (trim_arr + 18) << 8 | (int16_t) * (trim_arr + 17);
  NVM_PAR_P10 = *(trim_arr + 19);
  NVM_PAR_P11 = *(trim_arr + 20);

  PAR_T1 = (double)NVM_PAR_T1 * 256; // 1/2^-8
  PAR_T2 = (double)NVM_PAR_T2 / 1073741824; // 2^30
  PAR_T3 = (double)NVM_PAR_T3 / pow(2, 48);

  PAR_P1 = ((double)NVM_PAR_P1 - 16384) / 1048576; // 2^14 // 2^20
  PAR_P2 = ((double)NVM_PAR_P2 - 16384) / pow(2, 29); // 2^29
  PAR_P3 = (double)NVM_PAR_P3 / pow(2, 32);
  PAR_P4 = (double)NVM_PAR_P4 / pow(2, 37);
  PAR_P5 = (double)NVM_PAR_P5 * 8; // 1/2^-3
  PAR_P6 = (double)NVM_PAR_P6 / 64; // 2^6
  PAR_P7 = (double)NVM_PAR_P7 / 256;  // 2^7
  PAR_P8 = (double)NVM_PAR_P8 / 32768;  // 2^15
  PAR_P9 = (double)NVM_PAR_P9 / pow(2, 48);
  PAR_P10 = (double)NVM_PAR_P10 / pow(2, 48);
  PAR_P11 = (double)NVM_PAR_P11 / pow(2, 65);

}

/*

*/
bool BMP3XX_Sensor::BMP388_Get_Data(float* sensData) {
  uint8_t sensorData[BMP388_DATA_LENGHT_BYTES];
  uint32_t uncompDataTemp, uncompDataPress, data_msb, data_lsb, data_xlsb;
  bool rslt = false;

  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_DATA_0, sensorData, BMP388_DATA_LENGHT_BYTES);
  if (!rslt)
    return false;

  data_xlsb = sensorData[0];
  data_lsb = sensorData[1] << 8;
  data_msb = sensorData[2] << 16;
  uncompDataPress = data_msb | data_lsb | data_xlsb;

  data_xlsb = sensorData[3];
  data_lsb = sensorData[4] << 8;
  data_msb = sensorData[5] << 16;
  uncompDataTemp = data_msb | data_lsb | data_xlsb;

  float compTemp = BMP388_Compensate_Temperature(uncompDataTemp);
  float compPress = BMP388_Compensate_Pressure(uncompDataPress, compTemp);

  *sensData = compTemp;
  *(sensData + 1) = compPress;
  
  return true;
}

/*

*/
float BMP3XX_Sensor::BMP388_Compensate_Temperature(uint32_t uncomp_temp) {
  double partial_data1;
  double partial_data2;

  partial_data1 = (double)(uncomp_temp - PAR_T1);
  partial_data2 = (double)(partial_data1 * PAR_T2);

  t_lin = partial_data2 + (partial_data1 * partial_data1) * PAR_T3;
  return t_lin;

}

/*

*/
float BMP3XX_Sensor::BMP388_Compensate_Pressure(uint32_t uncomp_press, float comp_temp) {

  float comp_press;

  float partial_data1;
  float partial_data2;
  float partial_data3;
  float partial_data4;
  float partial_out1;
  float partial_out2;

  partial_data1 = PAR_P6 * comp_temp;
  partial_data2 = PAR_P7 * (comp_temp * comp_temp);
  partial_data3 = PAR_P8 * (comp_temp * comp_temp * comp_temp);
  partial_out1 = PAR_P5 + partial_data1 + partial_data2 + partial_data3;

  partial_data1 = PAR_P2 * comp_temp;
  partial_data2 = PAR_P3 * (comp_temp * comp_temp);
  partial_data3 = PAR_P4 * (comp_temp * comp_temp * comp_temp);
  partial_out2 = (float)uncomp_press * (PAR_P1 + partial_data1 + partial_data2 + partial_data3);

  partial_data1 = (float)uncomp_press * (float)uncomp_press;
  partial_data2 = PAR_P9 + PAR_P10 * comp_temp;
  partial_data3 = partial_data1 * partial_data2;
  partial_data4 = partial_data3 + ((float)uncomp_press * (float)uncomp_press * (float)uncomp_press) * PAR_P11;
  comp_press = partial_out1 + partial_out2 + partial_data4;

  return comp_press;
}

bool BMP3XX_Sensor::BMP388_Set_Options() {

  uint8_t rslt, reg, *regPtr = &reg, dat;

  /////////////////////////////////////////////////////////////////////
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_PWR_CTRL, regPtr, 1);
  if (!rslt) {
    Serial.println(" Error 1 ");
    return false;
  }

  delay(50);

  Serial.print("register PWR data: ");
  Serial.println(reg, HEX);
  dat = reg | (0x3);
  Serial.print("Sending data: ");
  Serial.println(dat, HEX);
  rslt = I2C_Send_Data(I2C_Address, BMP388_PWR_CTRL, dat);
  if (!rslt) {
    Serial.println(" Error 2 ");
    return false;
  }
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_OSR, regPtr, 1);
  if (!rslt) {
    Serial.println(" Error 1 ");
    return false;
  }

  delay(50);

  Serial.print("register OSR data: ");
  Serial.println(reg, HEX);
  dat = reg | (0xB);  // 2x temp 8x press
  Serial.print("Sending data: ");
  Serial.println(dat, HEX);
  rslt = I2C_Send_Data(I2C_Address, BMP388_OSR, dat);
  if (!rslt) {
    Serial.println(" Error 2 ");
    return false;
  }
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_ODR, regPtr, 1);
  if (!rslt) {
    Serial.println(" Error 1 ");
    return false;
  }

  delay(50);

  Serial.print("register ODR data: ");
  Serial.println(reg, HEX);
  dat = reg | (0x4);    // 80ms sampling period
  Serial.print("Sending data: ");
  Serial.println(dat, HEX);
  rslt = I2C_Send_Data(I2C_Address, BMP388_ODR, dat);
  if (!rslt) {
    Serial.println(" Error 2 ");
    return false;
  }
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_PWR_CTRL, regPtr, 1);
  if (!rslt) {
    Serial.println(" Error 1 ");
    return false;
  }

  delay(50);

  Serial.print("register PWR data: ");
  Serial.println(reg, HEX);
  dat = reg | (0x3 << 4);   // normal mode turn on
  Serial.print("Sending data: ");
  Serial.println(dat, HEX);
  rslt = I2C_Send_Data(I2C_Address, BMP388_PWR_CTRL, dat);
  if (!rslt) {
    Serial.println(" Error 2 ");
    return false;
  }
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_CONFIG, regPtr, 1);
  if (!rslt) {
    Serial.println(" Error 1 ");
    return false;
  }

  delay(50);

  Serial.print("register CONFIG data: ");
  Serial.println(reg, HEX);
  dat = reg | (0x1 << 1);  // 3 filter coeff
  Serial.print("Sending data: ");
  Serial.println(dat, HEX);
  rslt = I2C_Send_Data(I2C_Address, BMP388_CONFIG, dat);
  if (!rslt) {
    Serial.println(" Error 2 ");
    return false;
  }
  /////////////////////////////////////////////////////////////////////

  I2C_Read_Data_Bytes(I2C_Address, BMP388_PWR_CTRL, regPtr, 1);
  Serial.printf("Read PWR Reg %d\n\r", reg);
  I2C_Read_Data_Bytes(I2C_Address, BMP388_OSR, regPtr, 1);
  Serial.printf("Read OSR Reg %d\n\r", reg);
  I2C_Read_Data_Bytes(I2C_Address, BMP388_ODR, regPtr, 1);
  Serial.printf("Read ODR Reg %d\n\r", reg);
  I2C_Read_Data_Bytes(I2C_Address, BMP388_PWR_CTRL, regPtr, 1);
  Serial.printf("Read PWR Reg %d\n\r", reg);
  I2C_Read_Data_Bytes(I2C_Address, BMP388_CONFIG, regPtr, 1);
  Serial.printf("Read CONFIG Reg %d\n\r", reg);

  return true;
}
