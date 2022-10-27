#include "src/ElectricityCalc.h"
#include "src/config.h"
#include "src/wifi/accessPoint.h"


int roundToN(double value, int n) {
  return int(value / n) * n;
}

ElectricityCalc electricityCalc(CURRENT_INPUT_PIN);

void setup() {
  Serial.begin(115200);
  delay(1000);
  IPAddress ip = initAP(AP_SSID, AP_PWD);
  if (DEBUG) {
    Serial.print("Access point set up. IP address = ");
    Serial.println(ip);
  }
}

void loop() {
  electricityCalc.updateValues();
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(3000);
}
