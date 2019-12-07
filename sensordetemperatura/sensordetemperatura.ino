#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads1115(0x48);
const float At=0.16; //valor del diferencial de Tamb y T'amb
const float A=0.79; //valor de A en la ecuación V=A+BT
const float B=0.0348; //valor de B en la ecuación V=A+BT
  
void setup() {
  Serial.begin(9600);
  delay(800);
  Serial.println("Inicializando...");
  ads1115.begin(); //Initialize ads1115
  Serial.println("Ajustando la ganancia...");
  ads1115.setGain(GAIN_ONE);
  
  Serial.println("Tomando medidas del canal AIN0");
  
  Serial.println("Rango del ADC: +/- 4.096V (1 bit=2mV)");
  
}

void loop() {
  int16_t adc2;
  float voltaje;
  float temperatura;
  adc2 = ads1115.readADC_SingleEnded(2);
  voltaje=(adc2*3.3)/26405;
  temperatura = At+((voltaje-A)/B);
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print("ºC          AIN2: ");
  Serial.print(adc2);
  Serial.print("       Voltaje: ");
  Serial.println(voltaje);
  delay(1000);
}
