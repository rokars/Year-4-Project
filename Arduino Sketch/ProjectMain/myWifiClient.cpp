
#include "Arduino.h"  // Standard arduino functions
#include "myWifiClient.h"
#include <WiFi.h>

bool wifiClient_Init() {

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(homeSSID);

  Serial.println("test1******************");

  WiFi.begin(homeSSID, homePass);

  Serial.println("test2******************");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("test3******************");

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  return true;
}


bool wifiClient_SendDat() {

  Serial.print("connecting to ");
  Serial.println(homePcHost);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(homePcHost, httpPort)) {
    Serial.println("connection failed");
    return false;
  }

  // We now create a URI for the request
  String url = "/sendHeight";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + homePcHost + "\r\n" +
               "Connection: close\r\n\r\n");
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
  return true;
}
