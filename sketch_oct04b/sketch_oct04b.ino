#include "WiFi.h" 
#include "AsyncUDP.h" 
#include <TimeLib.h> 
#include <ArduinoJson.h>

const char * ssid = "TP-LINK_6CAE"; 
const char * password = "41422915";
AsyncUDP udp; 
StaticJsonDocument<200> jsonBuffer;              //tamaño maximo de los datos

void setup() { 
  Serial.begin(115200); 
  setTime (9, 15, 0, 7, 10, 2018); //hora minuto segundo dia mes año
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
      Serial.write(packet.data(), packet.length()); Serial.println();
    });
  }
}

void loop() { 
  delay(1000); 
  char texto[200];
  jsonBuffer["Hora"]=hour(); //Datos introducidos en el objeto “jsonbuffer" 
  jsonBuffer["Minuto"]=minute();  //3 campos 
  jsonBuffer["Segundo"]=second(); //
  serializeJson(jsonBuffer, texto); //paso del objeto “jsonbuffer" a texto para //transmitirlo 
  udp.broadcastTo(texto,1234);  //se envía por el puerto 1234 el JSON //como texto
}
