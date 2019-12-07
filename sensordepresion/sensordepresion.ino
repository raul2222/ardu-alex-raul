#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 presion;

#define ALTITUD 22.0 // Altitud en Gandía

void setup() {
  Serial.begin(9600);
  Serial.println("REBOOT");

  //Inicialización del sensor

  if(presion.begin())
    Serial.println("BMP180 init success");
  else{
    Serial.println("BMP180 init fail\n\n");
    while(1); //pausar indefinidamente
  }
}

void loop() {
  char status;
  double T,P,p0,a;

  //Lectura de presión cada 10 segundos

  Serial.println();
  Serial.print("Altitud obtenida: ");
  Serial.print(ALTITUD,0);
  Serial.println(" metros");

  //Empezamos la medida de temperatura para medir la presión

  status = presion.startTemperature();
  if (status != 0){
    delay(status);
    status= presion.getTemperature(T);
    if (status !=0){
      Serial.print("Temperatura: ");
      Serial.print(T,2);
      Serial.println(" deg C");
      status=presion.startPressure(3);
      if (status != 0){
        delay(status);
        status=presion.getPressure(P,T);
        if (status != 0){
          Serial.print("Presion absouluta: ");
          Serial.print(P,2);
          Serial.println(" mb");

          p0 = presion.sealevel(P,ALTITUD);
          Serial.print("Presion relativa (nivel del mar): ");
          Serial.print(p0,2);
          Serial.println(" mb");

          a = presion.altitude(P,p0);
          Serial.print("Altitud computada: ");
          Serial.print(a,0);
          Serial.println(" metros");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error retrieving pressure measurement\n");
    }
    else Serial.println("error retrieving pressure measurement\n");
  }
  else Serial.println("error retrieving pressure measurement\n");

  delay(5000);
}
