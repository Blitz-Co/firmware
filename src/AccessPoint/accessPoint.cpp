#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include "AccessPoint.h"

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

bool initFS() {
  return LittleFS.begin();
}

bool createAP(const char* ssid, const char* pwd) {
  return WiFi.softAP(ssid, pwd);
}

void obtainWifiCredentials() {
  server.on("/", HTTP_GET, mHandleIndexGet);
  server.on("/", HTTP_POST, mHandleIndexPost);
  server.begin();

  bool listen = true;
  while (listen) {
     server.handleClient();
  }
}
