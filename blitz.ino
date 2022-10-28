#include "src/ElectricityCalc/ElectricityCalc.h"
#include "src/AccessPoint/AccessPoint.h"
#include "src/config.h"


void setup() {
  Serial.begin(115200);
  // Create access point with given ssid and pwd, host a lcoal web page and obtain wi-fi credentials from the form
  WifiCredentials credentials = obtainWifiCredentials(AP_SSID, AP_PWD);
  Serial.print("SSID: ");
  Serial.println(credentials.ssid);
  Serial.print("PWD: ");
  Serial.println(credentials.pwd);
}

void loop() {
  
}
