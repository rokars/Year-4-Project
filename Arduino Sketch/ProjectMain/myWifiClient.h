
/*
   myWifiClient.h - Library for connecting
   posting data via WiFi,
   Created by Rokas Cesiunas, 24 APR, 2021
*/

#ifndef MYWIFICLIENT_H
#define MYWIFICLIENT_H

#include "Arduino.h"

const char ssid[] = "VM0061221";
const char password[] = "pcH7fjyv7dxc";
const char serverName[] = "http://192.168.0.64:4000/app/postData";

uint8_t wifiClient_Init();
uint8_t wifiClient_PostReq(float*, unsigned char*);

#endif
