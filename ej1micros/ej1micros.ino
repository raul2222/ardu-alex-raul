#include <M5Stack.h>

#define TX1_pin 21
#define RX1_pin -1

#define TX2_pin -1
#define RX2_pin 22

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("\n\n\n\n");
  M5.Lcd.printf("      Test 3 UARTs\r\n");
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX1_pin, TX1_pin);
  Serial2.begin(115200, SERIAL_8N1, RX2_pin, TX2_pin);
  Serial.println("Inicio");
}

char dato;
uint8_t num_bytes;

void loop() {
  // put your main code here, to run repeatedly:
  num_bytes = Serial.available();
  //Serial.println(num_bytes);
  if(num_bytes>0){
    while(num_bytes>0){
      num_bytes -= 1;
      dato = Serial.read();
      Serial1.print(dato);
      //Serial.println("entra 1");
    }
  }
  num_bytes = Serial2.available();
  //Serial.print("serial 2: ");
  //Serial.println(num_bytes);
  if(num_bytes>0){
    while(num_bytes>0){
      num_bytes -= 1;
      dato = Serial2.read();
      Serial.print(dato);
    }
  }
}
