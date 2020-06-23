#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define FIREBASE_HOST 
#define FIREBASE_AUTH 
#define listLen 17

ESP8266WebServer server(80);
FirebaseData firebaseData;

const char* ssid = ""; //Enter Wi-Fi SSID
const char* password = ""; //Enter Wi-Fi Password

int i = 0; int n = 0; String readGPSList[listLen];
char readGPSChar; String readGPS = "";
bool started = 0; bool packetReady = 0;
String dataDiscrip[] = {"msgID: ", "Time: ", "Lat: ", "NS: ", "Long: ", "EW: ", "Quality: ", "numSV: ", "HDOP: ", "Alt: ", "uAlt: ", "Sep: ", "uSep: ", "diffAge: ", "diffStation: ", "cs: ", "CRLF: "};

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    server.on("/", handle_index); //Handle Index page
    server.begin(); //Start the server

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

String latString = ""; String lonString = "";
String latPost = "F"; String lonPost = "F";

void loop() {
    server.handleClient(); //Handling of incoming client requests
    while (Serial.available()) {

        readGPSChar = (Serial.read());
        if (!started) {readGPS.concat(readGPSChar);}
        if (!started && readGPS.substring(3) == "GGA") {started=1;}
        if (started  && (readGPSChar == ',' || readGPSChar == '\n')) {readGPSList[n] = readGPS; readGPS=""; n++;}
        if (started  && (readGPSChar != ',' && readGPSChar != '\n')) {readGPS.concat(readGPSChar);}
        if (readGPSChar == '\n') {n=0; started=0; readGPS=""; packetReady=1; break;}
    }
    latString = readGPSList[2].substring(0, 2) + " " + readGPSList[2].substring(2) + " " + readGPSList[3];
    lonString = readGPSList[4].substring(0, 3) + " " + readGPSList[4].substring(3) + " " + readGPSList[5];

    if (Firebase.setString(firebaseData, "latitude:", latString))  {latPost="T";} else {latPost="F";}
    if (Firebase.setString(firebaseData, "longitude:", lonString)) {lonPost="T";} else {lonPost="F";}
    
}

void handle_index() {
  String output = "";
  if (packetReady) {
      for (i=0; i<listLen; i++) {
          output += dataDiscrip[i] + readGPSList[i] + "\n";
      }
      output += latString + "\n" + lonString + "\n";
      output += latPost + " " + lonPost + "\n";
      server.send(200, "text/plain", output);
  }
}
