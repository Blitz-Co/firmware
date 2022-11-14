#include "ESP8266WiFi.h"
#include "../config.h"
#include "Server.h"

bool connectWiFi(const char* ssid, const char* pwd, int timeout) { // Connect to WiFI and set wiFiConnected variable accordingly
    WiFi.begin((char*)ssid, (char*)pwd);
    if (DEBUG) {
        Serial.print("Connecting to wifi with ssid=");
        Serial.print(ssid);
        Serial.print(", password=");
        Serial.println(pwd);
    }
    bool messageShown = false;
    int startTime = millis();
    while (millis() - startTime < timeout) {
        if (WiFi.status() == WL_CONNECTED) {
            if (clientFetched) {
                return true;
            }
            wiFiConnected = true;
            
        }
        server.handleClient();
    }
    wiFiConnected = false;
    if (DEBUG) Serial.println("Failed connecting to WiFi due to timeout.");
    //server.close(); // Reset the server
    return false;
}