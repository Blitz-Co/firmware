#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include "AccessPoint.h"
#include "../config.h"

ESP8266WebServer server(80);

WifiCredentials credentials;
bool credentialsObtained = false;

WifiCredentials::WifiCredentials(const char* wifi_ssid, const char* wifi_pwd) {
  ssid = wifi_ssid;
  pwd = wifi_pwd;
}

WifiCredentials::WifiCredentials() {}


char* readFile(const char* fn) {
  File file = LittleFS.open(fn, "r");
  if (file) {
    size_t fileSize = file.size();
    char* fileBuffer = new char[fileSize + 1];
    int i = 0;
    for (int i = 0; i < fileSize; i++) {
      fileBuffer[i] = char(file.read());
    }
    fileBuffer[fileSize] = '\0';
    file.close();
    return fileBuffer;
  }
  return NULL;
}

void handleIndexGet() {
  char* fileContents = readFile("index.html");
  server.send(200, "text/html", fileContents); 
  delete[] fileContents;
}

void handleIndexPost() {
  char* fileContents;
  if (server.hasArg("ssid") && server.hasArg("pwd")) {
    credentials.ssid = server.arg("ssid").c_str();
    credentials.pwd = server.arg("pwd").c_str();
    fileContents = readFile("connecting.html");
    server.send(200, "text/html", fileContents);
    delay(1500);
    credentialsObtained = true;
  } else {
    fileContents = readFile("error.html");
    server.send(400, "text/html", fileContents);
  }
  delete[] fileContents;
}

WifiCredentials obtainWifiCredentials(const char* ap_ssid, const char* ap_pwd) {
  bool initializedFS = LittleFS.begin();
  bool created = WiFi.softAP(ap_ssid, ap_pwd);
  if (DEBUG) {
    Serial.print("LittleFS initialized = ");
    Serial.println(initializedFS);
    Serial.print("AP created = ");
    Serial.println(created);
  }
  if (created && initializedFS) {
    server.on("/", HTTP_GET, handleIndexGet);
    server.on("/", HTTP_POST, handleIndexPost);
    server.begin();

    while (!credentialsObtained) {
      server.handleClient();
    }
    WiFi.softAPdisconnect(true);
    server.stop();
  }
  server.close();
  return credentials;
}