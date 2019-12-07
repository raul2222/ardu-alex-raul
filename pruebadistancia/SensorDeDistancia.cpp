#include "SensorDeDistancia.h"
#import <Arduino.h>

SensorDeDistancia::SensorDeDistancia(){
  EchoPin = 36;
  //EchoPin = 5;
  TriggerPin = 26;
  //TriggerPin = 4;
  ledPin = 16;
  //ledPin = 2;
  freq = 5000;
  ledChannel = 0;
  resolution = 8;
  dutyCycle = 0;
  encendido = 0;
}

void SensorDeDistancia::begin(){
  pinMode(TriggerPin, OUTPUT);    
  pinMode(EchoPin, INPUT);
  //analogWrite(ledPin, 0);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}

int SensorDeDistancia::distancia(){
  long duracion, distanciaCm;    
  digitalWrite(TriggerPin, LOW); //nos aseguramos señal baja al principio    
  delayMicroseconds(4);    
  digitalWrite(TriggerPin, HIGH); //generamos pulso de 10us    
  delayMicroseconds(10);    
  digitalWrite(TriggerPin, LOW);    
  duracion = pulseIn(EchoPin, HIGH);  //medimos el tiempo del pulso
  //Serial.println(duracion);    
  distanciaCm = duracion * 10 / 292 / 2;   //convertimos a distancia    
  return distanciaCm; 
}

void SensorDeDistancia::toggleLuz(){
  // esp32
  Serial.println(dutyCycle);
  if(dutyCycle == 0 || dutyCycle == -1){
    // increase the LED brightness
    for(dutyCycle; dutyCycle <= 255; dutyCycle++){   
      // changing the LED brightness with PWM
      ledcWrite(ledChannel, dutyCycle);
      //analogWrite(ledPin,dutyCycle);
      encendido = 1;
      delay(15);
    }
  } else if(dutyCycle > 0 && dutyCycle <= 256){
    // decrease the LED brightness
    for(dutyCycle; dutyCycle >= 0; dutyCycle=dutyCycle-1){
      // changing the LED brightness with PWM
      ledcWrite(ledChannel, dutyCycle);
      //analogWrite(ledPin, dutyCycle);
      encendido = 0;   
      delay(15);
    }
  } 
  /*if(dutyCycle == 0 || dutyCycle == -1){
  // increase the LED brightness
  for(dutyCycle; dutyCycle < 1023; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(15);
  }
  } else if(dutyCycle > 0 && dutyCycle <= 1023){
  // decrease the LED brightness
  for(dutyCycle; dutyCycle > 0; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(15);
  }
  }*/
}

bool SensorDeDistancia::getEncendido(){
  return encendido;
}

void SensorDeDistancia::setEncendido(bool toggle){
  encendido = toggle;
}
