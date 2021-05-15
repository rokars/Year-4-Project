
/*
   myWifiClient.cpp- Library for connecting
   posting data via WiFi,
   Created by Rokas Cesiunas, 24 APR, 2021
*/

#include "Arduino.h"  // Standard arduino functions
#include "myWifiClient.h"
#include "Function_Return_Codes.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

/*
  Initialise and connect to WiFi 
*/ 
uint8_t wifiClient_Init() {

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to the WiFi network with IP: ");
  Serial.println(WiFi.localIP());

  return SENSOR_OK;
}

/*
  Send a post request to the specified Host
  accepts a pointer to a 10 float array that holds sensor data
  and a char pointer for an array that stores gps data 
*/ 
uint8_t wifiClient_PostReq(float *sensorData, unsigned char* gpsData) {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    // Use WiFiClient class to create TCP connections
    HTTPClient http;

    StaticJsonDocument<384> Sensor_Data_0;  // json size in ram see https://arduinojson.org/v6/assistant/

    Sensor_Data_0["Magnetometer_X"] = * sensorData;;
    Sensor_Data_0["Magnetometer_Y"] = * (sensorData + 1);
    Sensor_Data_0["Magnetometer_Z"] = * (sensorData + 2);
    Sensor_Data_0["Gyroscope_X"] = * (sensorData + 3);
    Sensor_Data_0["Gyroscope_Y"] = * (sensorData + 4);
    Sensor_Data_0["Gyroscope_Z"] = * (sensorData + 5);
    Sensor_Data_0["Accelerometer_X"] = * (sensorData + 6);
    Sensor_Data_0["Accelerometer_Y"] = * (sensorData + 7);
    Sensor_Data_0["Accelerometer_Z"] = * (sensorData + 8);
    Sensor_Data_0["Temperature"] = * (sensorData + 9);
    Sensor_Data_0["Pressure"] = * (sensorData + 10);
    Sensor_Data_0["GPS_String"] = gpsData;

    String json;
    serializeJson(Sensor_Data_0, json);

    http.begin(serverName);  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");             //Specify content-type header
    //http.addHeader("Content-Length", json.length());

    int httpResponseCode = http.POST(json);   //Send the actual POST request

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();  //Free resources

    return SENSOR_OK;
  }
  else
    return COMM_ERR;
}
