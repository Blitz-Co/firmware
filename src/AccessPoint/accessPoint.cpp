#include <ESP8266WiFi.h>

IPAddress initAP(const char* ssid, const char* pwd) {
  WiFi.softAP(ssid, pwd);
  return WiFi.softAPIP();
}

void disconnectAP() {
  WiFi.softAPdisconnect(true);
}