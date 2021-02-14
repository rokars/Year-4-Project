
#include <Wire.h>
#include <cstdint>
#include "BMP3XX_Sensor.h"
#include "I2C_Transmit.h"

// I2C address when SD0 = VDDIO
uint8_t BMP388_Address_Primary = 0x77;

bool rslt;
BMP3XX_Sensor bmp(BMP388_Address_Primary);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  Serial.printf("\n\rSize of float: %d\n\rSize of double: %d\n\r", sizeof(float), sizeof(double));

  bool rslt = bmp.BMP3XX_Board_Init();
  Serial.printf("\n\r result of board init: %d\n\r", rslt);
  //I2C_Send_Data(0x77, 0x1C, 0b00001011);
  //delay(10);
  rslt = bmp.BMP388_Set_Options();
  Serial.printf("\n\r result of set options: %d\n\r", rslt);
  delay(50);
  uint8_t dat, *datPtr = &dat;
  I2C_Read_Data_Bytes(0x77, 0x02, datPtr, 1);
  Serial.print(" ERR REGISTER CHECK: ");
  Serial.println(dat, BIN); 
}

void loop() {
  // put your main code here, to run repeatedly:
  //bmp.BMP388_Set_Options();
  bmp.BMP388_Get_Data();
  delay(50);

}
