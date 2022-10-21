#include "ElectricityCalc.h"

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
