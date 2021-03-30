#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "VM0061221";
const char* password = "pcH7fjyv7dxc";
const char* serverName = "http://192.168.0.64:8000/sendHeight";

const String json = "{\"mock_data\": [{\"TimeStampDate\": \"2020-12-26T13:55:12Z\",\"AltitudeQnh_meters\": 85,\"AirSpeed_knots\": -24.3,\"VerticalSpeed_ms\": 4.08,\"NormalAcceleration_g\": 0.85,\"MotorPower_volts\": 3.4}]}";
uint8_t jsonLen = json.length();

void setup() {

  Serial.begin(115200);
  delay(4000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to the WiFi network with IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;


    http.begin(serverName);  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");             //Specify content-type header
    http.addHeader("Content-Length", String(jsonLen));

    int httpResponseCode = http.POST(json);   //Send the actual POST request

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);


    http.end();  //Free resources
  }
  else {
    Serial.println("Error in WiFi connection");
  }

  delay(5000);  //Send a request every 5 seconds
}
