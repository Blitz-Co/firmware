#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include "Server.h"
#include "../config.h"

ESP8266WebServer server(80);

WifiCredentials credentials; // Credentials of the local wifi network
bool credentialsObtained = false; // Indicates whether the credentials have beebn obtained
bool wiFiConnected = false; // Indicates whether the credentials provided were correct
bool clientFetched = false; // Indicates whether when wifi connects successfully, the client has fetched that
bool initialized = false; // Indicates whether LittleFS has been initialized and soft ap created

WifiCredentials::WifiCredentials(const char* wifi_ssid, const char* wifi_pwd) {
  ssid = wifi_ssid;
  pwd = wifi_pwd;
}

WifiCredentials::WifiCredentials() {}


char* readFile(const char* fn) { // Reads file from LittleFS and returns the buffer, remember to free memory afterwards
  File file = LittleFS.open(fn, "r"); // Open in read mode
  if (file) {
    size_t fileSize = file.size();
    char* fileBuffer = new char[fileSize + 1]; // Allocate file buffer
    int i = 0;
    // Read file into buffer
    for (int i = 0; i < fileSize; i++) {
      fileBuffer[i] = char(file.read());
    }
    fileBuffer[fileSize] = '\0'; // Terminate the buffer
    file.close(); // Close the file
    return fileBuffer;
  }
  return NULL;
}

void handleIndexGet() { // Handles GET / and serves index.html
  char* fileContents = readFile("index.html");
  server.send(200, "text/html", fileContents); 
  delete[] fileContents;
}

void handleIndexPost() { // Handles POST / and serves connecting.html
  char* fileContents;
  Serial.print("IN POST ssid=");
  Serial.print(server.arg("ssid").c_str());
  Serial.print(", pwd=");
  Serial.println(server.arg("pwd").c_str());
  if (server.hasArg("ssid") && server.hasArg("pwd")) { // If ssid and pwd in post body
    credentials.ssid = server.arg("ssid").c_str(); // Read ssid
    credentials.pwd = server.arg("pwd").c_str(); // Read password
    fileContents = readFile("connecting.html");
    server.send(200, "text/html", fileContents);
    credentialsObtained = true;
  } else {
    // If ssid and password is not present in the post body, then serve error.html
    fileContents = readFile("error.html");
    server.send(400, "text/html", fileContents);
  }
  delete[] fileContents;
}

void handleConnectionGet() {
  if (wiFiConnected) clientFetched = true;
  server.send(200, "text/html", String(wiFiConnected));
}

// Create access point with given ssid and pwd, host a lcoal web page and obtain wi-fi credentials from the form
WifiCredentials obtainWifiCredentials(const char* ap_ssid, const char* ap_pwd) {
  if (!initialized) {
    bool initializedFS = LittleFS.begin(); // Initialize the file system
    bool created = WiFi.softAP(ap_ssid, ap_pwd); // Create access point
    bool successful = initializedFS && created;
    if (successful) {
      server.on("/", HTTP_GET, handleIndexGet);
      server.on("/", HTTP_POST, handleIndexPost);
      server.on("/isConnected", HTTP_GET, handleConnectionGet);
      server.begin();
    }
    initialized = true;
    Serial.print("LittleFS initialized = ");
    Serial.println(initializedFS);
    Serial.print("AP created = ");
    Serial.print(created);
    Serial.print(", ip=");
    Serial.println(WiFi.softAPIP());
  }
  
  credentials = WifiCredentials();
  credentialsObtained = false;

  while (!credentialsObtained) {
    server.handleClient();
  }
  return credentials;
}