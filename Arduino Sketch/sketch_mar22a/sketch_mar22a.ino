#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "VM0061221";
const char* password = "pcH7fjyv7dxc";
const char* serverName = "http://192.168.0.64:8090/sendHeight";

String json = "{\"mock_data\": [{\"TimeStampDate\": \"2020-12-26T13:55:12Z\",\"AltitudeQnh_meters\": 85,\"AirSpeed_knots\": -24.3,\"VerticalSpeed_ms\": 4.08,\"NormalAcceleration_g\": 0.85,\"MotorPower_volts\": 3.4}]}";

void setup() {

  Serial.begin(115200);
  delay(4000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
  if (WiFi.status() == WL_CONNECTED) {  //Check WiFi connection status

    HTTPClient http;

    http.begin(serverName);

    //http.addHeader("Content-Type", "application/json");
    http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST(json);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

  delay(5000);
}
