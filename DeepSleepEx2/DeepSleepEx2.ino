#define BLANCO 0XFFFF 
#define NEGRO 0 
#define ROJO 0xF800 
#define VERDE 0x07E0 
#define AZUL 0x001F 
//#include <M5Stack.h>

//RTC_DATA_ATTR definicion de variable en memoria del RTC 
RTC_DATA_ATTR int contador = 0;

void setup() { 
  // put your setup code here, to run once: 
  //M5.begin(); 
  Serial.begin(9600);
  delay(1000);
  //M5.Lcd.setTextSize(3); //Tamaño del texto 
  pinMode(39, INPUT_PULLUP); //pin 39 entrada con resistencia de pullup
  contador++; 
  //M5.Lcd.setCursor(0, 10);    //posicion inicial del cursor 
  //M5.Lcd.setTextColor(BLANCO);  //color del texto 
  //M5.Lcd.print(contador);
  Serial.println(contador); 
  delay (1000); 
  //programación de la entrada 39 (Boton A del M5Stack) para despertar 
  //al ESP32 con un nivel bajo (al pulsarlo) 
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39,0); //1 = High, 0 = Low 
  esp_deep_sleep_start(); //duerme al ESP32 (modo SLEEP) 
}

void loop(){
  
}
