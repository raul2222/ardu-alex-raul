#include <WiFi.h>
#include <TimeLib.h>
#include "time.h"

const char* ssid = "TP-LINK_6CAE"; //red wifi 
const char* password = "41422915"; //contraseña
const char* ntpServer = "hora.rediris.es"; 
const long gmtOffset_sec = 3600; 
const int daylightOffset_sec = 3600;

void printLocalTime() { 
  struct tm timeinfo; 
  if(!getLocalTime(&timeinfo)){ 
    Serial.println("Failed to obtain time"); 
    return; 
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S"); 
  Serial.println(&timeinfo, "%H:%M:%S"); 
  int Hora=timeinfo.tm_hour; 
  Serial.println(Hora); 
}

void setup() { 
  Serial.begin(9600); //connect to WiFi 
  Serial.printf("Connecting to %s ", ssid); 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 
  Serial.println(" CONNECTED"); //init and get the time 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); 
  printLocalTime(); //disconnect WiFi as it's no longer needed 
  WiFi.disconnect(true); 
  WiFi.mode(WIFI_OFF);
}

void loop() { 
  delay(1000); 
  printLocalTime(); 
}
