
/*
  BMP3XX_Sensor.h - Library for connecting and receiving data from
  Bosch BMP3XX family sensors,
  Created by Rokas Cesiunas, 09 FEB 2021.
*/

#include "Arduino.h"  // Standard arduino functions
#include "BMP3XX_Sensor.h"
#include "I2C_Transmit.h"
#include "Function_Return_Codes.h"
#include <Wire.h>     // I2C communication
#include <cstdint>    // C style integers

/*
   Constructor
   bool SD0 referring to I2C address jumper on board
   true = high, false = low
*/
BMP3XX_Sensor::BMP3XX_Sensor(bool SD0) {
  if (SD0)
    I2C_Address = BMP388_ADDRESS_PRIMARY;
  else
    I2C_Address = BMP388_ADDRESS_SECONDARY;
}

/*
   Self-test
   Soft reset -> read chip ID -> read trimming data
   Returns Function Return Code
*/
uint8_t BMP3XX_Sensor::BMP3XX_Board_Init() {

  uint8_t sensorInfo = 0;
  uint8_t* sensorInfoPtr = &sensorInfo;
  bool rslt = false;
  uint8_t rsltFunc = COMM_ERR;

  //  Soft reset sensor to erase any possible old settings
  rslt = I2C_Send_Data(I2C_Address, BMP388_CMD, BMP388_SOFTRESET);
  if (!rslt)
    return COMM_ERR;

  //  Delay to give time to reset
  uint8_t count = 5;
  do {
    delay(5);
    count--;
    if (count == 0)
      return COMM_ERR;
  } while (BMP388_Command_Ready() == READING_ERR);

  //  Query Event register for power on reset complete (por_detected)
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_EVENT, sensorInfoPtr, ONE_BYTE);
  if (!rslt || !sensorInfo)
    return COMM_ERR;

  // Read chip ID number to ensure good communication
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_CHIP_ID, sensorInfoPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  else if (sensorInfo != BMP388_CHIP_ID_NO)
    return DEV_ID_ERR;

  //  Read Trimming Coefficients from Register
  uint8_t trim_data[BMP388_CALIB_DATA_LENGHT_BYTES];
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_CALIB_DATA_START, trim_data, BMP388_CALIB_DATA_LENGHT_BYTES);
  if (!rslt)
    return COMM_ERR;

  rsltFunc = BMP3XX_Trim_Data_Parse(trim_data);
  if (rsltFunc != SENSOR_OK)
    return READING_ERR;

  //  set to forced mode for one measurement
  I2C_Send_Data(I2C_Address, BMP388_PWR_CTRL, BMP388_FORCED_MODE);
  delay(50);

  // check error register for errors
  if (BMP388_Check_Err() != SENSOR_OK)
    return DEVICE_ERR;

  // Test data read is within limits
  float sensorTestData[2];
  
  rsltFunc = BMP388_Get_Data(sensorTestData);
  if (rsltFunc != SENSOR_OK)
    return READING_ERR;

  if (sensorTestData[0] >= TEMP_UPPER_LIM_C && sensorTestData[0] <= TEMP_LOWER_LIM_C)
    return TEMP_ERR;
  if (sensorTestData[1] >= PRESS_UPPER_LIM_HPA && sensorTestData[1] <= PRESS_LOWER_LIM_HPA)
    return PRESS_ERR;

  return SENSOR_OK;
}

/*
   Returns true if BMP388 sensor is ready to receive commands
   Returns Function Return Code
*/
uint8_t BMP3XX_Sensor::BMP388_Command_Ready() {

  uint8_t rslt = 0, sensorStatus, *stPtr = &sensorStatus;
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_STATUS, stPtr, 1);
  if (rslt) {
    sensorStatus = sensorStatus >> 4;
    if (sensorStatus & 0b0001 == 1)
      return SENSOR_OK;
    else
      return READING_ERR;
  }
}

/*
  Calculate and store the true trim data values to be used for correction of data readings 
  input parameter is an array that stores raw trimming values
  Returns value via reference
*/
uint8_t BMP3XX_Sensor::BMP3XX_Trim_Data_Parse(uint8_t *trim_arr) {

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

  // Formulas taken from BMP388 datasheet section 9.1 (page 55)
  // Adapted to remove some power functions to reduce mathematical computations required

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

  return SENSOR_OK;

}

/*
  Queries the sensor for new data readings
  input parameter is a float array to store returned measurement
  Applies compensation to data received
  Sets value via reference
  Returns Function Return Codes
*/
uint8_t BMP3XX_Sensor::BMP388_Get_Data(float* sensData) {
  
  uint8_t sensorData[BMP388_DATA_LENGHT_BYTES];
  uint32_t uncompDataTemp, uncompDataPress, data_msb, data_lsb, data_xlsb;
  bool rslt = false;

  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_DATA_0, sensorData, BMP388_DATA_LENGHT_BYTES);
  if (!rslt)
    return COMM_ERR;

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

  return SENSOR_OK;
}

/*
  Formulas taken from BMP388 datasheet section 9.2 (page 55)
  input parameter is uncompensated temperature returned from raw sensor reading
  Applies formula outlined in datasheet to raw temp readings with trimming values
  Returns compensated temperature
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
  Formulas taken from BMP388 datasheet section 9.3 (page 56)
  input parameter is compensated temperature, and uncompensated pressure returned from raw sensor reading
  Applies formula outlined in datasheet to raw press readings with trimming values
  Returns compensated pressure
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

/*
  Sents the configuration for running the sensor
  Returns Function Return Code
 */
uint8_t BMP3XX_Sensor::BMP388_Set_Options() {

  bool rslt = false;
  uint8_t reg, *regPtr = &reg, dat;

  // Set sleep mode and enable temperature and pressure sensors
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_PWR_CTRL, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  delay(50);
  
  dat = reg | BMP388_TEMP_PRESS_ON_SLEEP;
  rslt = I2C_Send_Data(I2C_Address, BMP388_PWR_CTRL, dat);
  if (!rslt)
    return CONFIG_ERR;


  // Set Oversampling to 2 times for temperature and 8 times for pressure
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_OSR, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  delay(50);

  dat = reg | BMP388_2XTEMP_8XPRESS_OVERSAMPLE;
  rslt = I2C_Send_Data(I2C_Address, BMP388_OSR, dat);
  if (!rslt)
    return CONFIG_ERR;


  // Set the sampling period to 640ms
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_ODR, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  delay(50);

  dat = reg | BMP388_640MS_ODR;
  rslt = I2C_Send_Data(I2C_Address, BMP388_ODR, dat);
  if (!rslt)
    return CONFIG_ERR;


  // Enable normal mode for taking measurements
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_PWR_CTRL, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  delay(50);

  dat = reg | BMP388_NORMAL_MODE;
  rslt = I2C_Send_Data(I2C_Address, BMP388_PWR_CTRL, dat);
  if (!rslt)
    return CONFIG_ERR;


  // Set the IIR filter coefficient to 3
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_CONFIG, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  delay(50);

  dat = reg | BMP388_IIR_FILTER_COEFF_3;
  rslt = I2C_Send_Data(I2C_Address, BMP388_CONFIG, dat);
  if (!rslt)
    return CONFIG_ERR;

  return SENSOR_OK;
}

/*
  Reading all BMP388 registers that are configured when setting up
  Used for checking the register data set is correct
  Returns Function Return Code
 */
uint8_t BMP3XX_Sensor::BMP388_Read_Config() {

  bool rslt = false;
  uint8_t reg, *regPtr = &reg;
  
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_PWR_CTRL, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  Serial.printf("Read PWR configuration register %d\n\r", reg);
  
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_OSR, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  Serial.printf("Read OSR configuration register %d\n\r", reg);
  
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_ODR, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  Serial.printf("Read ODR configuration register %d\n\r", reg);
  
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_PWR_CTRL, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  Serial.printf("Read PWR configuration register %d\n\r", reg);
  
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_CONFIG, regPtr, ONE_BYTE);
  if (!rslt)
    return COMM_ERR;
  Serial.printf("Read CONFIG configuration register %d\n\r", reg);
  
  return SENSOR_OK;
}

/*
  Testing sensor error register for values higher than zero
  Returns Function Return Code
 */
uint8_t BMP3XX_Sensor::BMP388_Check_Err() {

  uint8_t sensorInfo = 0;
  uint8_t* sensorInfoPtr = &sensorInfo;
  bool rslt = false;

  // test err reg
  rslt = I2C_Read_Data_Bytes(I2C_Address, BMP388_ERR_REG, sensorInfoPtr, ONE_BYTE);
  if (rslt) {
    if (sensorInfo & BMP_FATAL_ERR == BMP_FATAL_ERR) {
      return DEVICE_ERR;
    } else if (sensorInfo & BMP_CMD_ERR == BMP_CMD_ERR) {
      return COMM_ERR;
    } else if (sensorInfo & BMP_CONF_ERR == BMP_CONF_ERR) {
      return CONFIG_ERR;
    } else
      return SENSOR_OK;
  }
}
