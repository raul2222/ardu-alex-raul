#include <M5Stack.h>

const int MQ_PIN = A0;
const int MQ_DELAY = 2000;

float array[200];
float mayor, menor, media;
 
void setup()
{
  M5.begin();
  Serial.begin(9600);
  for(int i=0;i<=199;i++){
    int raw_adc = analogRead(MQ_PIN);
    float value_adc = raw_adc * (5.0 / 1023.0);
    M5.Lcd.setCursor(0, 0, 2);

  // Set the font colour to be white with a black background, set text size multiplier to 1

  M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);  
    M5.Lcd.setTextSize(1);
    M5.Lcd.println("Hello World!");
    array[i]=value_adc;
    delay(10);
  }
  mayor=array[0];
  for(int i=1;i<=199;i++){
    if(array[i]>mayor){
      mayor=array[i];
    }
  }
  menor=array[0];
  for(int i=0;i<=199;i++){
    if(array[0]<menor){
      menor=array[i];
    }
  }
  float aux=0;
  for(int i=0;i<=199;i++){
    aux=aux+array[i];
  }
  media=aux/200;
}
 
void loop() 
{
  if(M5.BtnA.wasPressed()){
    Serial.print("Raw:");
    Serial.print(raw_adc);
    Serial.print("    Tension:");
    Serial.println(value_adc);
  }
  
}
