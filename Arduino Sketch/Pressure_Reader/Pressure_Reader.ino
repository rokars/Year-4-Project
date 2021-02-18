
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
  if (!rslt) {
    Serial.println("\n\r *** ERROR board init");
  }
  delay(50);

  rslt = bmp.BMP388_Set_Options();
  if (!rslt) {
    Serial.println("\n\r *** ERROR set options");
  }
  delay(50);

  uint8_t dat, *datPtr = &dat;
  I2C_Read_Data_Bytes(0x77, 0x02, datPtr, 1);
  Serial.print(" ERR REGISTER CHECK: ");
  Serial.println(dat, BIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  float sensor_Data_Out[2], sumTemp = 0, sumPress = 0, tempRead, pressRead, altitude;
  bool rslt = false;

  for (int i = 0; i < 10; i++) {
    rslt = bmp.BMP388_Get_Data(sensor_Data_Out);
    if (!rslt) {
      Serial.println(" Read Error ");
    }
    sumTemp += sensor_Data_Out[0];
    sumPress += sensor_Data_Out[1];
    delay(80);
  }
  tempRead = sumTemp / 10;
  pressRead = sumPress / 10;
  Serial.printf(" temperature (C): %0.2f, pressure (PA): %0.2f ", tempRead, pressRead);

  altitude = 145366.45 * (1 - pow(((pressRead / 100) / 1005.422), 0.190284));
  Serial.printf("Approx. altitude %0.2f\n\r", altitude / 3.281);

}
