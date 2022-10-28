#include "ElectricityCalc.h"

#define CALIBRATION 111.1

ElectricityCalc::ElectricityCalc(int pin) {
  emon.current(pin, CALIBRATION); // Initialize energy monitor
}

void ElectricityCalc::updateValues() {
  // Calculate power by taking the area under the energy function curve
  int startTime = millis();
  m_calcIrms();
  m_calcEnergy();
  int endTime = millis();
  double wms = energy * (endTime - startTime); // watt milliseconds
  double kwh = wms / 3600000 / 1000; // kilowatt hours
  power += kwh;
}

void ElectricityCalc::m_calcIrms() {
  irms = emon.calcIrms(1480); // Sample size is 1480
}

void ElectricityCalc::m_calcEnergy() {
  energy = irms * voltage;
}
