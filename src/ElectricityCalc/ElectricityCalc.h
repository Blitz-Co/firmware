#pragma once

#include "EmonLib.h"

class ElectricityCalc {
public:
  ElectricityCalc(int pin);
  double voltage = 230.0; // 230 V is assumed
  double irms; // Root mean square current
  double energy; // In watts
  double power = 0; // In kilowatt hours
  EnergyMonitor emon;

public:
  void updateValues();
  int roundEnergyToN(int n);
  
private:
  void m_calcIrms();
  void m_calcEnergy();
};
