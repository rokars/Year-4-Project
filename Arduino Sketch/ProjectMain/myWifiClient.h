
/*
   SIM28_GPS_Receiver.h - Library for connecting and
   receiving data from SIM28 GPS Receiver
   Created by Rokas Cesiunas 11 MAR, 2021
*/

#ifndef MYWIFICLIENT_H
#define MYWIFICLIENT_H

#include "Arduino.h"

const char ssid[] = "VM0061221";
const char password[] = "pcH7fjyv7dxc";
const char serverName[] = "http://192.168.0.64:8000/sendHeight";

bool wifiClient_Init();
bool wifiClient_GetReq();
bool wifiClient_PostReq();

#endif
