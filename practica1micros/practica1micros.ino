#include <M5Stack.h>

#define Pin 36

const float k = 0.000887573964497;

float raw_value[200];

int i = 0;

void setup() {
  // put your setup code here, to run once:
  M5.begin(true,false,true);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  Serial.begin(9600);
  pinMode(Pin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(i==200){
    i=0;
    delay(10);
  }
  int a;
  double v;
  a = analogRead(Pin);
  v = k * a;
  raw_value[i] = a;
  if(Serial.available()){
    char command = (char) Serial.read();
    if(command == 'a'){
      Serial.print("Raw: ");Serial.print(raw_value[i]);Serial.print("  Voltaje: ");Serial.println(v);
      double media = procesarMedia(raw_value);
      double maximo = procesarMaximo(raw_value); 
    }
  }
  /*if(i==99||i==199){
    M5.Lcd.setCursor(0,16);
    M5.Lcd.printf("Raw: %f", lecturas[i].raw_value);
    M5.Lcd.setCursor(0,32);
    M5.Lcd.printf("Voltaje %f", lecturas[i].voltaje);
    M5.update();
  }*/
  //Serial.print("Tensión: ");
  //Serial.println(v);
  i++;
  delay(10);
}

double procesarMedia(float lecturas[]){
  double aux[200];
  for(int i = 0; i<=199; i++){
    aux[i] = lecturas[i] * k;
  }
  double aux2 = 0;
  for(int k = 0; k<=199; k++){
    aux2 = aux2 + aux[i];
  }
  double media = aux2 / 200;
  return media;
}

double procesarMaximo(float lecturas[]){
  double aux[200];
  for(int i = 0; i<=199; i++){
    aux[i] = lecturas[i] * k;
  }
  double aux2 = aux[0];
  for(int k = 1; k<=199; k++){
    
  }
}
