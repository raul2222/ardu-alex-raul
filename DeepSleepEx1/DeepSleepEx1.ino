#define BLANCO 0XFFFF 
#define NEGRO 0 
#define ROJO 0xF800 
#define VERDE 0x07E0 
#define AZUL 0x001F 
//#include <M5Stack.h>

//RTC_DATA_ATTR definicion de variable en memoria del RTC 
RTC_DATA_ATTR int bootCount = 0;

//factor de conversion de microsegundos a segundos
#define uS_TO_S_FACTOR 1000000  //se salva como 32bits 
//tiempo que el ESP32 estara dormido (en segundos) 
#define TIME_TO_SLEEP 5

//#define uS_TO_S_FACTOR 1000000ULL  //se salva como 64bits 
//#define TIME_TO_SLEEP 5ULL

void setup() {
  Serial.begin(9600);
  //M5.begin(); 
  //M5.Lcd.setTextSize(3); //Tamaño del texto 
  ++bootCount; 
  Serial.println(bootCount);
  //M5.Lcd.setCursor(0, 10);    //posicion inicial del cursor 
  //M5.Lcd.setTextColor(BLANCO);  //color del texto 
  //M5.Lcd.print(bootCount); 
  delay (1000); 
  //programacion del temporizador del RTC para que despierte 
  //al ESP32 al cabo del tiempo definido 
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); 
  esp_deep_sleep_start(); //duerme al ESP32 (modo SLEEP)
}

void loop() {
  // put your main code here, to run repeatedly:

}
