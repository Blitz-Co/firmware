#pragma once

class WifiCredentials {
public:
    WifiCredentials(const char* wifi_ssid, const char* wifi_pwd);
    const char* ssid;
    const char* pwd;
};

bool initFS();
bool createAP(const char* ssid, const char* pwd);
void obtainWifiCredentials();