// 01 - Web server on your Wi-Fi (station mode)
// Board: Waveshare ESP32-P4-WIFI6-DEV-KIT. Wi-Fi comes from the onboard ESP32-C6
// via ESP-Hosted, built into the Arduino esp32 core. The C6 is 2.4 GHz only.
// STATUS: worked on my board (page loaded from phone; reachable from laptop).

#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Hello from ESP32-P4!</h1>");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.setSleep(false);   // steadier ping/response times
  Serial.println();
  Serial.print("Open: http://");   // use http://, not https://
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
