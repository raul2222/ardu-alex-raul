#include "SensorDeDistancia.h"
#include "WiFi.h" 
#include "AsyncUDP.h" 

// SENSOR:
// ESP32 || ESP8266
// Vcc -> 5V || -> Vin
// Trig -> 26 || -> D2
// Echo -> SVP || -> D1
// GND -> GND
// --------------------
// LED:
// CORTO -> GND
// LARGO -> 16 || -> D4

SensorDeDistancia sdd;

const char * ssid = "TEAM_1"; 
const char * password = "123456789";

AsyncUDP udp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sdd.begin();
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password); 
  if (WiFi.waitForConnectResult() != WL_CONNECTED) { 
    Serial.println("WiFi Failed"); 
    while(1) { 
      delay(1000); 
    } 
  }
  if(udp.listen(1234)) { 
    Serial.print("UDP Listening on IP: "); 
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) { 
      Serial.write(packet.data(), packet.length()); 
      Serial.println(); 
    });
  }
  delay(2500);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool hayCambio;
  int distancia = sdd.distancia();
  Serial.println(distancia);
  if(distancia<30){
    sdd.toggleLuz();
    hayCambio = 1;
  }else{
    hayCambio = 0;
  }
  if(hayCambio){
    if(sdd.getEncendido()){
      Serial.println("lo he encendido");
      delay(1000);
      udp.broadcastTo("encendido",1234);
      hayCambio = 0;
    } else{
      Serial.println("lo he apagado");
      delay(1000);
      udp.broadcastTo("apagado",1234);
      hayCambio = 0;
    }
  } else{
    Serial.println("no hay cambio");
  }
  delay(2000);
}
