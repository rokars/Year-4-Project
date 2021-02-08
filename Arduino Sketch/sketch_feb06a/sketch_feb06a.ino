
#include <Wire.h>
#include <cstdint>

// I2C address when SD0 = VDDIO
uint8_t BMP388_Address_Primary = 0x77;

// BMP388 REGISTER ADDRESSES

uint8_t BMP388_CHIP_ID = 0x00;
uint8_t BMP388_ERR_REG = 0x02;
uint8_t BMP388_STATUS = 0x03;

uint8_t BMP388_DATA_0 = 0x04;
uint8_t BMP388_DATA_1 = 0x05;
uint8_t BMP388_DATA_2 = 0x06;
uint8_t BMP388_DATA_3 = 0x07;
uint8_t BMP388_DATA_4 = 0x08;
uint8_t BMP388_DATA_5 = 0x09;

uint8_t BMP388_EVENT = 0x10;
uint8_t BMP388_PWR_CTRL = 0x1B;
uint8_t BMP388_OSR = 0x1C;
uint8_t BMP388_ODR = 0x1D;
uint8_t BMP388_CONFIG = 0x1F;

//  MEMORY MAP TRIMMING COEFFICIENTS
uint8_t BMP388_CALIB_DATA_START = 0x31;
byte BMP388_CALIB_DATA_LENGHT = 21;

uint8_t BMP388_CMD = 0x7E;
//  BMP388 Commands
uint8_t BMP388_softreset = 0xB6;
// BMP Constants
uint8_t BMP388_CHIP_ID_NO = 0x50;


//
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

//
byte PRESS_XLSB_7_0;
byte PRESS_LSB_15_8;
byte PRESS_MSB_23_16;
byte TEMP_XLSB_7_0;
byte TEMP_LSB_15_8;
byte TEMP_MSB_23_16;

/*     */

byte reg_data[] = {PRESS_XLSB_7_0, PRESS_LSB_15_8,
                   PRESS_MSB_23_16, TEMP_XLSB_7_0,
                   TEMP_LSB_15_8, TEMP_MSB_23_16
                  };

uint32_t dataReceive1;
uint32_t dataReceive2;
long t_lin;
//long dataReceive3;
bool rslt;

float PAR_T1;
float PAR_T2;
float PAR_T3;
float PAR_P1;
float PAR_P2;
float PAR_P3;
float PAR_P4;
float PAR_P5;
float PAR_P6;
float PAR_P7;
float PAR_P8;
float PAR_P9;
float PAR_P10;
float PAR_P11;

void BMP388_compensate_temperature(uint32_t);
bool BMP388_command_ready();
bool BMP3XX_Board_Init(uint8_t);
void BMP3XX_TRIM_DATA_VERIFY(uint8_t, uint8_t, byte);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  rslt = BMP3XX_Board_Init(BMP388_Address_Primary, BMP388_CALIB_DATA_START, BMP388_CALIB_DATA_LENGHT);

  Serial.printf("\n\rResult: %d\n\r", rslt);

  //////////////////////////////////////////////////////

  /*Serial.print("\n\rReading register: ");
  Serial.print(BMP388_ERR_REG, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_ERR_REG, HEX);
  Serial.println(" hex");

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_ERR_REG));
  if (Wire.endTransmission())
    Serial.println(" ACK received ");

  Wire.requestFrom(byte(BMP388_Address_Primary), 1);
  Serial.print("Error Register reading: ");
  if (Wire.available() <= 1) {
    dataReceive1 = Wire.read();
  }
  Serial.println(dataReceive1, BIN);

  if (dataReceive1 >= 1 && dataReceive1 <= 7) {
    Serial.println("SENSOR ERROR");
  }
  delay(100);

  //////////////////////////////////////////////////////

  Serial.print("\n\rWriting to register: ");
  Serial.print(BMP388_PWR_CTRL, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_PWR_CTRL, HEX);
  Serial.println(" hex");
  Serial.print(" Value to be written: ");
  Serial.print(0b00110010, BIN);  //0x33
  Serial.print(" binary, ");
  Serial.print(0b00110010, HEX);
  Serial.println(" hex");

  Serial.print("\n\rWriting to register: ");
  Serial.print(BMP388_OSR, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_OSR, HEX);
  Serial.println(" hex");
  Serial.print(" Value to be written: ");
  Serial.print(0b00000011, BIN);  //0x03
  Serial.print(" binary, ");
  Serial.print(0b00000011, HEX);
  Serial.println(" hex");

  Serial.print("\n\rWriting to register: ");
  Serial.print(BMP388_CONFIG, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_CONFIG, HEX);
  Serial.println(" hex");
  Serial.print(" Value to be written: ");
  Serial.print(0b00000001, BIN);
  Serial.print(" binary, ");
  Serial.print(0b00000001, HEX);
  Serial.println(" hex");

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(BMP388_OSR);
  Wire.write(0b00000011);   //set over sampling: temp x1, press x8
  Wire.endTransmission(false);
  if (Wire.endTransmission())
    Serial.println(" ACK received ");
  delay(50);

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_PWR_CTRL));
  Wire.write(0b00110010);   //set power mode to normal, temp enabled
  Wire.endTransmission(false);
  if (Wire.endTransmission())
    Serial.println(" ACK received ");
  delay(50);

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_CONFIG));
  Wire.write(0b00000001);   //iir filter coeff 1
  Wire.endTransmission();
  if (Wire.endTransmission())
    Serial.println(" ACK received ");
  delay(50);

  //////////////////////////////////////////////////////

  Serial.print("\n\rReading registers: ");
  Serial.print(BMP388_PWR_CTRL, BIN);
  Serial.print(" (");
  Serial.print(BMP388_PWR_CTRL, HEX);
  Serial.println(") ");

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_PWR_CTRL));
  Wire.endTransmission(false);
  if (Wire.endTransmission())
    Serial.println(" ACK received ");

  Wire.requestFrom(BMP388_Address_Primary, 2);
  if (Wire.available() == 2) {
    dataReceive1 = Wire.read();
    dataReceive2 = Wire.read();
  }

  Serial.print("Power Ctrl register: ");
  Serial.print(dataReceive1, BIN);
  Serial.print(" OSR register: ");
  Serial.println(dataReceive2, BIN);

  Serial.println("\n\rReading temp data registers: ");
  */
  //////////////////////////////////////////////////////
}

void loop() {
  // put your main code here, to run repeatedly:

  /*dataReceive1 = 0;
    dataReceive2 = 0;
    //dataReceive3 = 0;

    Wire.beginTransmission(byte(BMP388_Address_Primary));
    Wire.write(byte(DATA_3));
    Wire.endTransmission(false);
    Wire.requestFrom(BMP388_Address_Primary, 3);

    if (Wire.available() == 3 ) {
    dataReceive1 = Wire.read();
    Serial.print("DATA_3: ");
    Serial.print(dataReceive1, BIN);
    dataReceive1 = dataReceive1 << 16;
    dataReceive2 = Wire.read();
    Serial.print("DATA_4: ");
    Serial.print(dataReceive1, BIN);
    dataReceive2 = dataReceive2 << 8;
    dataReceive1 |= dataReceive2;
    dataReceive1 = Wire.read();
    }

    Serial.print("Temp data_3 - data_5: ");
    Serial.print(dataReceive1, BIN);
    Serial.print(" binary, ");
    Serial.print(dataReceive1, HEX);
    Serial.print(" hex, decimal: ");
    Serial.println(dataReceive1, DEC);
    BMP388_compensate_temperature(dataReceive1);


  

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_DATA_0));
  Wire.endTransmission(false);
  Wire.requestFrom(byte(BMP388_Address_Primary), 6);

  if (Wire.available() == 6 ) {
    PRESS_XLSB_7_0 = Wire.read();
    PRESS_LSB_15_8 = Wire.read();
    PRESS_MSB_23_16 = Wire.read();
    TEMP_XLSB_7_0 = Wire.read();
    TEMP_LSB_15_8 = Wire.read();
    TEMP_MSB_23_16 = Wire.read();
  }

  Serial.print("Temperature data: ");
  Serial.print(" d3: ");
  Serial.print(TEMP_XLSB_7_0, BIN);
  Serial.print(" d4: ");
  Serial.print(TEMP_LSB_15_8, BIN);
  Serial.print(" d5: ");
  Serial.println(TEMP_MSB_23_16, BIN);

  dataReceive1 = 0;
  dataReceive2 = 0;

  Serial.print("shifted: ");
  dataReceive1 = TEMP_MSB_23_16 << 16;
  Serial.print("data MSB: ");
  Serial.println(dataReceive1, BIN);

  dataReceive2 = TEMP_LSB_15_8 << 8;
  Serial.print("data LSB: ");
  Serial.println(dataReceive2, BIN);

  dataReceive1 |= dataReceive2;
  Serial.print("data MSB |= LSB: ");
  Serial.println(dataReceive1, BIN);

  dataReceive1 |= reg_data[3];
  Serial.print("Temp???: ");
  Serial.println(dataReceive1, BIN);

  //BMP388_compensate_temperature(dataReceive1);

  delay(1000);*/

}

void BMP388_compensate_temperature(uint32_t uncomp_temp) {
  float partial_data1;
  float partial_data2;

  partial_data1 = (float)(uncomp_temp - PAR_T1);
  partial_data2 = (float)(partial_data1 * PAR_T2);

  t_lin = partial_data2 + (partial_data1 * partial_data1) * PAR_T3;

  Serial.print("t_lin compensated data?: ");
  //Serial.print(t_lin, BIN);
  //Serial.print(" binary, ");
  //Serial.print(t_lin, HEX);
  Serial.print(" decimal: ");
  Serial.println(t_lin);

}

bool BMP388_command_ready() {

  uint8_t STATUS;

  Wire.begin(BMP388_Address_Primary);
  Wire.write(byte(0x03));
  Wire.endTransmission();

  Wire.requestFrom(byte(BMP388_Address_Primary), 1);
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

bool BMP3XX_Board_Init(uint8_t I2C_ADDRESS, uint8_t CALIB_DATA_START, uint8_t CALIB_DATA_lEN) {

  uint8_t dataIn;
  uint8_t trim_data[CALIB_DATA_lEN];

  //  Soft reset sensor to erase any possible old settings
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(BMP388_CMD));
  Wire.write(byte(BMP388_softreset));
  if (Wire.endTransmission() > 0) {
    Serial.printf("\n\r *** Error Connecting to 0x%x *** \n\r", I2C_ADDRESS);
    return false;
  }
  Serial.printf("\n\rPerformed softreset on BMP sensor 0x%x \n\r", I2C_ADDRESS);
  delay(5);

  // Read chip id number to ensure good communication
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(BMP388_CHIP_ID));
  if (Wire.endTransmission(false) > 0) { // do not send stop messsage
    Serial.println("\n\r no ack ");
    return false;
  }
  Wire.requestFrom(I2C_ADDRESS, 1);
  if (Wire.available() == 1) {
    dataIn = Wire.read();
    if (dataIn != BMP388_CHIP_ID_NO) {
      Serial.printf("\n\rRead incorrect Chip ID 0x%x stopping\n\r", dataIn);
      return false;
    }
  }

  //  Read Trimming Coefficients from Register   //
  Serial.println("\n\rReading Trimming Coefficients ");

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(CALIB_DATA_START));
  if (Wire.endTransmission(false) > 0) {
    Serial.println("\n\r no ack ");
    return false;
  }

  Wire.requestFrom(I2C_ADDRESS, CALIB_DATA_lEN);
  if (Wire.available() == CALIB_DATA_lEN) {
    for (byte i = 0; i < CALIB_DATA_lEN; i++) {
      trim_data[i] = Wire.read();
    }
  }
  BMP3XX_TRIM_DATA_VERIFY(trim_data);
  return true;
}

void BMP3XX_TRIM_DATA_VERIFY(uint8_t trimming_data[]) {

  for (byte i = 0; i < BMP388_CALIB_DATA_LENGHT; i++) {
    Serial.printf("NVM_TRIM_DATA[%d]: 0x%x [ ", i, trimming_data[i]);
    Serial.print(trimming_data[i], BIN);
    Serial.printf(" ]\n\r");
  }

  /*NVM_PAR_T1 = Wire.read();
    NVM_PAR_T1 = NVM_PAR_T1 << 8;
    NVM_PAR_T1 |= Wire.read();

    NVM_PAR_T2 = Wire.read();
    NVM_PAR_T2 = NVM_PAR_T2 << 8;
    NVM_PAR_T2 |= Wire.read();

    NVM_PAR_T3 = Wire.read();

    NVM_PAR_P1 = Wire.read();
    NVM_PAR_P1 = NVM_PAR_P1 << 8;
    NVM_PAR_P1 |= Wire.read();

    NVM_PAR_P2 = Wire.read();
    NVM_PAR_P2 = NVM_PAR_P2 << 8;
    NVM_PAR_P2 |= Wire.read();

    NVM_PAR_P3 = Wire.read();

    NVM_PAR_P4 = Wire.read();

    NVM_PAR_P5 = Wire.read();
    NVM_PAR_P5 = NVM_PAR_P5 << 8;
    NVM_PAR_P5 |= Wire.read();

    NVM_PAR_P6 = Wire.read();
    NVM_PAR_P6 = NVM_PAR_P6 << 8;
    NVM_PAR_P6 |= Wire.read();

    NVM_PAR_P7 = Wire.read();

    NVM_PAR_P8 = Wire.read();

    NVM_PAR_P9 = Wire.read();
    NVM_PAR_P9 = NVM_PAR_P9 << 8;
    NVM_PAR_P9 |= Wire.read();

    NVM_PAR_P10 = Wire.read();

    NVM_PAR_P11 = Wire.read();

    Serial.println(NVM_PAR_T1);
    Serial.println(NVM_PAR_T2);
    Serial.println(NVM_PAR_T3);
    Serial.println(NVM_PAR_P1);
    Serial.println(NVM_PAR_P2);
    Serial.println(NVM_PAR_P3);
    Serial.println(NVM_PAR_P4);
    Serial.println(NVM_PAR_P5);
    Serial.println(NVM_PAR_P6);
    Serial.println(NVM_PAR_P7);
    Serial.println(NVM_PAR_P8);
    Serial.println(NVM_PAR_P9);
    Serial.println(NVM_PAR_P10);
    Serial.println(NVM_PAR_P11);

    PAR_T1 = (NVM_PAR_T1) / (pow(2, -8));
    PAR_T2 = (NVM_PAR_T2) / (pow(2, 30));
    PAR_T3 = (NVM_PAR_T3) / (pow(2, 48));

    Serial.println("floating point compensated values: ");
    Serial.printf("\n\r%.6f, %.6f, %.6f\n\r", PAR_T1, PAR_T2, PAR_T3);*/
}



/*
  //////////////////////////////////////////////////////
  // Print I2C Address of which we are connnecting to //

  Serial.print("\n\rConnecting to slave with address: ");
  Serial.print(BMP388_Address_Primary, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_Address_Primary, HEX);
  Serial.println(" hex");
  delay(100);

  //////////////////////////////////////////////////////
  //          Print the CHIP_ID Register             //

  Serial.print("\n\rReading register: ");
  Serial.print(BMP388_CHIP_ID, BIN);
  Serial.print(" binary, ");
  Serial.print(BMP388_CHIP_ID, HEX);
  Serial.println(" hex");

  Wire.beginTransmission(byte(BMP388_Address_Primary));
  Wire.write(byte(BMP388_CHIP_ID));
  Wire.endTransmission();
  if (Wire.endTransmission())
    Serial.println(" ACK received ");

  Wire.requestFrom(byte(BMP388_Address_Primary), 1);
  Serial.print("Chip ID: ");
  if (Wire.available() == 1) {
    dataReceive1 = Wire.read();
  }
  Serial.print(dataReceive1, BIN);
  Serial.print(", in decimal: ");
  Serial.println(dataReceive1, DEC);
  delay(100);


*/
