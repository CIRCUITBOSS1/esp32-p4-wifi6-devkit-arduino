// 02 - Web server on the P4's own Wi-Fi network (SoftAP)
// Join "P4-Server" from your phone, then open http://192.168.4.1
// STATUS: NOT TESTED YET - test and update the README.

#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Hello from ESP32-P4 (AP mode)!</h1>");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("P4-Server", "12345678");   // password must be 8+ characters

  Serial.print("Connect to Wi-Fi 'P4-Server', then open http://");
  Serial.println(WiFi.softAPIP());        // usually 192.168.4.1

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
