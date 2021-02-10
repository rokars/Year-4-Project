
#include <Wire.h>
#include <cstdint>
#include "BMP3XX_Sensor.h"
#include "I2C_Transmit.h"

// I2C address when SD0 = VDDIO
uint8_t BMP388_Address_Primary = 0x77;

bool rslt;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  //rslt = BMP3XX_Board_Init(BMP388_Address_Primary, BMP388_CALIB_DATA_START, BMP388_CALIB_DATA_LENGHT);

  //Serial.printf("\n\rResult: %d\n\r", rslt);

  BMP3XX_Sensor bmp(BMP388_Address_Primary);

  bmp.BMP3XX_Board_Init();
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
