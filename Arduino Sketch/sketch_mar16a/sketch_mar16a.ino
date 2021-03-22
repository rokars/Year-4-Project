#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>
 
const char *AP_SSID = "VM0061221";
const char *AP_PWD = "pcH7fjyv7dxc";
  
WiFiMulti wifiMulti;
 
void setup() {
  Serial.begin(115200);
   
  delay(4000);
  wifiMulti.addAP(AP_SSID, AP_PWD);
 
  postDataToServer();
}
 
void loop() {
  // Not used in this example
}
 
void postDataToServer() {
 
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {
     
    HTTPClient http;   

    Serial.println("connected");
     
    http.begin("192.168.0.64:8000/doSomething");
    http.addHeader("Content-Type", "application/json");         
     
    StaticJsonDocument<200> doc;
    // Add values in the document
    //
    doc["sensor"] = "gps";
    doc["time"] = 1351824120;
   
    // Add an array
    JsonArray data = doc.createNestedArray("data");
    data.add(48.756080);
    data.add(2.302038);
     
    String requestBody;
    serializeJson(doc, requestBody);
     
    int httpResponseCode = http.POST(requestBody);
 
    if(httpResponseCode>0){
       
      String response = http.getString();                       
       
      Serial.println(httpResponseCode);   
      Serial.println(response);
     
    }
    else {
      //Serial.printf("Error occurred while sending HTTP POST: %s\n", httpClient.errorToString(statusCode).c_str());
    }
  }
}
