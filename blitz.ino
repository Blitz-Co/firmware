#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include "src/ElectricityCalc/ElectricityCalc.h"
#include "src/AccessPoint/AccessPoint.h"
#include "src/config.h"


void setup() {
  Serial.begin(115200);
  WifiCredentials credentials = obtainWifiCredentials(AP_SSID, AP_PWD);
  Serial.print("SSID: ");
  Serial.println(credentials.ssid);
  Serial.print("PWD: ");
  Serial.println(credentials.pwd);
}

void loop() {
  
}
