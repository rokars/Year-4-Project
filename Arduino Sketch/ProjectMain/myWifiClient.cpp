
#include "Arduino.h"  // Standard arduino functions
#include "myWifiClient.h"
#include <WiFi.h>
#include <HTTPClient.h>

bool wifiClient_Init() {

  // We start by connecting to a WiFi network

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to the WiFi network with IP: ");
  Serial.println(WiFi.localIP());

  return true;
}


bool wifiClient_PostReq(float *postData) {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    // Use WiFiClient class to create TCP connections
    HTTPClient http;

    const String json2 = String("{\"Sensor_Data\": [{\"Magnetometer_X\":") + * postData + ",\"Magnetometer_Y\":" + * (postData + 1) + ",\"Magnetometer_Z\":" + * (postData + 2) + ",\"Gyroscope_X\":" + * (postData + 3) + ",\"Gyroscope_Y\":" + * (postData + 4) + ",\"Gyroscope_Z\":" + * (postData + 5) + ",\"Accelerometer_X\":" + * (postData + 6) + ",\"Accelerometer_Y\":" + * (postData + 7) + ",\"Accelerometer_Z\":" + * (postData + 8) + ",\"Temperature\":" + * (postData + 9) + ",\"Pressure\":" + * (postData + 10) + "}]}";
    const uint8_t jsonLen = json2.length();

    http.begin(serverName);  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");             //Specify content-type header
    http.addHeader("Content-Length", String(jsonLen));

    int httpResponseCode = http.POST(json2);   //Send the actual POST request

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    http.end();  //Free resources
    
    return true;
  }
  else
    return false;
}

bool wifiClient_GetReq() {

  /*Serial.print("connecting to ");
    Serial.println(homePcHost);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(homePcHost, httpPort)) {
    Serial.println("connection failed");
    return false;
    }

    // We now create a URI for the request
    String url = "/doSomething";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    String jsonString = "{\"mock_data\": [{\"TimeStampDate\": \"2020-12-26T13:55:12Z\",\"AltitudeQnh_meters";

    // This will send the request to the server
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + homePcHost + "\r\n" +
               "{api_key: \"api\", field1: value1}" +
               "Content-Type: application/json");
    unsigned long timeout = millis();
    while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return false;
    }
    }

    // Read all the lines of the reply from server and print them to Serial
    if (client.available()) {
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    }
    Serial.println();
    Serial.println("closing connection");
    return true;*/
}
