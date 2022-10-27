#pragma once

#include <ESP8266WiFi.h>

IPAddress initAP(const char* ssid, const char* pwd);
void disconnectAP();