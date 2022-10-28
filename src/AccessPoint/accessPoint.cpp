#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include "AccessPoint.h"
#include "../config.h"

ESP8266WebServer server(80);


char* mReadFile(const char* fn) {
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

void mHandleIndexGet() {
  char* fileContents = mReadFile("index.html");
  server.send(200, "text/html", fileContents); 
  delete[] fileContents;
}

void mHandleIndexPost() {
  char* fileContents;
  if (server.hasArg("ssid") && server.hasArg("pwd")) {
    Serial.println(server.arg("ssid"));
    Serial.println(server.arg("pwd"));
    fileContents = mReadFile("connecting.html");
    server.send(200, "text/html", fileContents);
  } else {
    fileContents = mReadFile("error.html");
    server.send(400, "text/html", fileContents);
  }
  delete[] fileContents;
}

bool mInitFS() {
  return LittleFS.begin();
}

bool mCreateAP(const char* ssid, const char* pwd) {
  return WiFi.softAP(ssid, pwd);
}

void obtainWifiCredentials(const char* ap_ssid, const char* ap_pwd) {
  bool initialized = mInitFS();
  bool created = mCreateAP(ap_ssid, ap_pwd);
  if (DEBUG) {
    Serial.print("LittleFS initialized: ");
    Serial.println(initialized);
    Serial.print("AP created: ");
    Serial.println(created);
  }
  if (created && initialized) {
    server.on("/", HTTP_GET, mHandleIndexGet);
    server.on("/", HTTP_POST, mHandleIndexPost);
    server.begin();

    bool listen = true;
    while (listen) {
      server.handleClient();
    }
  }
}