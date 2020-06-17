#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = ""; //Enter Wi-Fi SSID
const char* password =  ""; //Enter Wi-Fi Password

int i = 0; int n = 0; String readGPSList[8];
char readGPSChar; String readGPS = "";
bool started = 0; bool packetReady = 0;

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    server.on("/", handle_index); //Handle Index page
    server.begin(); //Start the server

}

void loop() {
    server.handleClient(); //Handling of incoming client requests
    while (Serial.available()) {
        readGPSChar = (Serial.read());
        readGPS.concat(readGPSChar);
        if (!started && readGPS.substring(3) == "GGA") {started=1;}
        if (started  && readGPSChar == '\n') {readGPSList[n] = readGPS; n++;}
        if (readGPSChar == '\n') {readGPS="";}
        if (n == 8) {n=0; packetReady=1; break;}
    }
}

void handle_index() {
  //Print Hello at opening homepage
  String output = "";
  if (packetReady) {
      for (i=0; i<8; i++) {
          output += readGPSList[i];
      }
      server.send(200, "text/plain", output);
  }
  server.send(200, "text/plain", "Hello! This is an index page.");
}
