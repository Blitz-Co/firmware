#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include "AccessPoint.h"
#include "../config.h"

ESP8266WebServer server(80);

WifiCredentials credentials; // Credentials of the local wifi network
bool credentialsObtained = false; // Indicates whether the credentials have beebn obtained

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
  if (server.hasArg("ssid") && server.hasArg("pwd")) { // If ssid and pwd in post body
    credentials.ssid = server.arg("ssid").c_str(); // Read ssid
    credentials.pwd = server.arg("pwd").c_str(); // Read password
    fileContents = readFile("connecting.html");
    server.send(200, "text/html", fileContents);
    delay(1500); // Delay setting credentialsObtained variable to serve the connecting.html and only then stop the server
    credentialsObtained = true;
  } else {
    // If ssid and password is not present in the post body, then serve error.html
    fileContents = readFile("error.html");
    server.send(400, "text/html", fileContents);
  }
  delete[] fileContents;
}

// Create access point with given ssid and pwd, host a lcoal web page and obtain wi-fi credentials from the form
WifiCredentials obtainWifiCredentials(const char* ap_ssid, const char* ap_pwd) {
  bool initializedFS = LittleFS.begin(); // Initialize the file system
  bool created = WiFi.softAP(ap_ssid, ap_pwd); // Create access point
  if (DEBUG) {
    Serial.print("LittleFS initialized = ");
    Serial.println(initializedFS);
    Serial.print("AP created = ");
    Serial.println(created);
  }
  if (created && initializedFS) {
    // Serve web pages locally
    server.on("/", HTTP_GET, handleIndexGet);
    server.on("/", HTTP_POST, handleIndexPost);
    server.begin();

    // Serve while the credentials are not obtained
    while (!credentialsObtained) {
      server.handleClient();
    }
    WiFi.softAPdisconnect(true);
    server.stop();
  }
  server.close(); // Close the server afterwards
  return credentials;
}