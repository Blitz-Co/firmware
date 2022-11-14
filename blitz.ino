#include "src/ElectricityCalc/ElectricityCalc.h"
#include "src/Server/Server.h"
#include "src/config.h"

void setup() {
  Serial.begin(115200);
  bool credentialsCorrect = false;
  do {
    WifiCredentials credentials = obtainWifiCredentials(AP_SSID, AP_PWD);
    credentialsCorrect = connectWiFi(credentials.ssid, credentials.pwd, 15000);
  } while (!credentialsCorrect);
  
  Serial.println("WiFi connected...");
}

void loop() {
  
}
