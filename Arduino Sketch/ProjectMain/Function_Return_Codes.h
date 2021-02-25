
/*
  I2C_Transmit.h - Library for sending and receiving
  data through I2C protocol,
  Created by Rokas Cesiunas, 10 FEB 2021.
*/

#ifndef FUNCTION_RETURN_CODES
#define FUNCTION_RETURN_CODES

const uint8_t SENSOR_OK = 0;

const uint8_t COMM_ERR = 10;
const uint8_t DEV_ID_ERR = 11;
const uint8_t DEVICE_ERR = 20;
const uint8_t CONFIG_ERR = 30;
const uint8_t READING_ERR = 40;

const uint8_t TEMP_ERR = 20;
const uint8_t PRESS_ERR = 30;

const uint8_t BMP_FATAL_ERR = 1 << 0;
const uint8_t BMP_CMD_ERR = 1 << 1;
const uint8_t BMP_CONF_ERR = 1 << 2;

#endif
