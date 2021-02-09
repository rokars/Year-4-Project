
/*
  BMP3XX_Sensor.h - Library for connecting and receiving data from
  Bosch BMP3XX family sensors
  Created by Rokas Cesiunas, 09 FEB, 2021.
*/

#ifndef BMP3XX_SENSOR_H
#define BMP3XX_SENSOR_H

#include "Arduino.h"
#include <Wire.h>
#include <cstdint>

class BMP3XX_Sensor
{
  public:
    BMP3XX_Sensor(uint8_t address);
    bool BMP3XX_Board_Init();
    bool BMP388_Command_Ready();
    void BMP3XX_Trim_Data_Verify();
    void BMP388_Compensate_Temperature(uint32_t uncomp_temp);


  private:
    uint8_t I2C_Address;

    //  BMP388 REGISTER ADDRESSES
    const uint8_t BMP388_CHIP_ID = 0x00;
    const uint8_t BMP388_ERR_REG = 0x02;
    const uint8_t BMP388_STATUS = 0x03;

    const uint8_t BMP388_DATA_0 = 0x04; // PRESS_XLSB_7_0
    const uint8_t BMP388_DATA_1 = 0x05; // PRESS_LSB_15_8
    const uint8_t BMP388_DATA_2 = 0x06; // PRESS_MSB_23_16
    const uint8_t BMP388_DATA_3 = 0x07; // TEMP_XLSB_7_0
    const uint8_t BMP388_DATA_4 = 0x08; // TEMP_LSB_15_8
    const uint8_t BMP388_DATA_5 = 0x09; // PRESS_MSB_23_16

    const uint8_t BMP388_EVENT = 0x10;
    const uint8_t BMP388_PWR_CTRL = 0x1B;
    const uint8_t BMP388_OSR = 0x1C;
    const uint8_t BMP388_ODR = 0x1D;
    const uint8_t BMP388_CONFIG = 0x1F;

    const uint8_t BMP388_CALIB_DATA_START = 0x31;
    const uint8_t BMP388_CMD = 0x7E;

    //  BMP388 CMD register commands
    uint8_t BMP388_softreset = 0xB6;

    // BMP Constants
    const byte BMP388_CALIB_DATA_LENGHT = 21;
    const uint8_t BMP388_CHIP_ID_NO = 0x50;

    // Memory map trimming coefficients
    uint16_t NVM_PAR_T1 = 0;
    uint16_t NVM_PAR_T2 = 0;
    int8_t NVM_PAR_T3 = 0;

    int16_t NVM_PAR_P1 = 0;
    int16_t NVM_PAR_P2 = 0;
    int8_t NVM_PAR_P3 = 0;
    int8_t NVM_PAR_P4 = 0;
    uint16_t NVM_PAR_P5 = 0;
    uint16_t NVM_PAR_P6 = 0;
    int8_t NVM_PAR_P7 = 0;
    int8_t NVM_PAR_P8 = 0;
    int16_t NVM_PAR_P9 = 0;
    int8_t NVM_PAR_P10 = 0;
    int8_t NVM_PAR_P11 = 0;

    uint8_t dataReceived[];
};

#endif
