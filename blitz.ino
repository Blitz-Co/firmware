#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include "ElectricityCalc.h"
#include "config.h"

#define CURRENT_INPUT_PIN A0



int roundToN(double value, int n) {
  return int(value / n) * n;
}

ElectricityCalc electricityCalc(CURRENT_INPUT_PIN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  electricityCalc.updateValues();
  Serial.print(electricityCalc.power);
  Serial.println(" kwh");
  Serial.print(roundToN(electricityCalc.energy, 10));
  Serial.println(" W");
}
