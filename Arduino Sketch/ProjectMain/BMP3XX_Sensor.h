
/*
  BMP3XX_Sensor.h - Library for connecting and receiving data from
  Bosch BMP3XX family sensors
  Created by Rokas Cesiunas, 09 FEB, 2021.
*/

#ifndef BMP3XX_SENSOR_H
#define BMP3XX_SENSOR_H

#include "Arduino.h"  // Standard arduino functions
#include <Wire.h>     // I2C communication
#include <cstdint>    // C style integers

class BMP3XX_Sensor
{
  public:
    BMP3XX_Sensor(bool);                             // Contstructor
    uint8_t BMP3XX_Board_Init();                                   // Self-test
    bool BMP388_Command_Ready();                                // CMD decoder status
    void BMP3XX_Trim_Data_Parse(uint8_t*);                //
    float BMP388_Compensate_Temperature(uint32_t);
    float BMP388_Compensate_Pressure(uint32_t, float);  //
    bool BMP388_Get_Data(float*);                                     //
    bool BMP388_Set_Options();
    uint8_t BMP388_Check_Err();


  private:
    uint8_t I2C_Address;
    const uint8_t BMP388_ADDRESS_PRIMARY = 0x77;// I2C address when SD0 pin = VDDIO
    const uint8_t BMP388_ADDRESS_SECONDARY = 0x76;// I2C address when SD0 pin = GND

    //  BMP388 REGISTER ADDRESSES
    const uint8_t BMP388_CHIP_ID = 0x00;
    const uint8_t BMP388_ERR_REG = 0x02;
    const uint8_t BMP388_STATUS = 0x03;

    const uint8_t BMP388_DATA_0 = 0x04; // PRESS_XLSB_7_0
    const uint8_t BMP388_DATA_1 = 0x05; // PRESS_LSB_15_8
    const uint8_t BMP388_DATA_2 = 0x06; // PRESS_MSB_23_16
    const uint8_t BMP388_DATA_3 = 0x07; // TEMP_XLSB_7_0
    const uint8_t BMP388_DATA_4 = 0x08; // TEMP_LSB_15_8
    const uint8_t BMP388_DATA_5 = 0x09; // TEMP_MSB_23_16

    const uint8_t BMP388_EVENT = 0x10;
    const uint8_t BMP388_PWR_CTRL = 0x1B;
    const uint8_t BMP388_OSR = 0x1C;
    const uint8_t BMP388_ODR = 0x1D;
    const uint8_t BMP388_CONFIG = 0x1F;

    const uint8_t BMP388_CALIB_DATA_START = 0x31;
    const uint8_t BMP388_CMD = 0x7E;

    //  BMP388 CMD register commands
    uint8_t BMP388_softreset = 0xB6;

    //  BMP388 Constants
    const uint8_t BMP388_CALIB_DATA_LENGHT_BYTES = 21;
    const uint8_t BMP388_CHIP_ID_NO = 0x50;
    const uint8_t BMP388_DATA_LENGHT_BYTES = 6;

    //  Memory Map Trimming Coefficients
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

    //  Calculated Calibration Coefficients
    double PAR_T1;
    double PAR_T2;
    double PAR_T3;

    double PAR_P1;
    double PAR_P2;
    double PAR_P3;
    double PAR_P4;
    double PAR_P5;
    double PAR_P6;
    double PAR_P7;
    double PAR_P8;
    double PAR_P9;
    double PAR_P10;
    double PAR_P11;
    float t_lin;

    const uint8_t TEMP_UPPER_LIM_C = 40;
    const uint8_t TEMP_LOWER_LIM_C = 0;

    const uint16_t PRESS_UPPER_LIM_HPA = 1100;
    const uint16_t PRESS_LOWER_LIM_HPA = 900;

};

#endif
