#include <SoftwareSerial.h>
#define RX 12
#define TX 11

SoftwareSerial GPS_Serial(RX, TX);

void setup(){
    Serial.begin(9600);
    GPS_Serial.begin(9600);
}

int i = 0; int n = 0; String readGPSList[6];
char readGPSChar; String readGPS = "";
bool started = 0;
void loop(){
    //Serial.println("========================================");
    while (GPS_Serial.available()) {
        readGPSChar = (GPS_Serial.read());
        readGPS.concat(readGPSChar);
        if (!started && readGPS.substring(3) == "GGA") {started=1;}
        if (started  && readGPSChar == '\n') {readGPSList[n] = readGPS; Serial.print(readGPS); n++;}
        if (readGPSChar == '\n') {readGPS="";}
        if (n == 8) {n=0; Serial.println("ReadAll"); break;}
    }
}

