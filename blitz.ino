#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include "src/ElectricityCalc/ElectricityCalc.h"
#include "src/AccessPoint/AccessPoint.h"
#include "src/config.h"


void setup() {
  Serial.begin(115200);
  bool fsInitialized = initFS();
  bool apCreated = createAP(AP_SSID, AP_PWD);
  if (DEBUG) {
    Serial.print("LittleFS initialized: ");
    Serial.println(fsInitialized);
    Serial.print("SoftAP created: ");
    Serial.println(apCreated);
  }
  if (fsInitialized && apCreated) {
    obtainWifiCredentials();
  }
}

void loop() {
  
}
