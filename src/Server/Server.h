#pragma once

#include <ESP8266WebServer.h>

extern bool wiFiConnected;
extern ESP8266WebServer server;
extern bool clientFetched;

class WifiCredentials {
public:
    WifiCredentials();
    WifiCredentials(const char* wifi_ssid, const char* wifi_pwd);
    const char* ssid;
    const char* pwd;
};

WifiCredentials obtainWifiCredentials(const char* ap_ssid, const char* ap_pwd);
bool connectWiFi(const char* ssid, const char* pwd, int timeout); // timeout in millis