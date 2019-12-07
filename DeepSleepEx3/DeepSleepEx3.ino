#define BUTTON_PIN_BITMASK 0x0802000004  // IO 2 y 25 y 35 activas

//RTC_DATA_ATTR definicion de variable en memoria del RTC 
RTC_DATA_ATTR int contador = 0; 
RTC_DATA_ATTR int contador_t = 0; 
RTC_DATA_ATTR int contador1 = 0; 
RTC_DATA_ATTR int contador2 = 0;

//factor de conversion de microsegundos a segundos 
#define uS_TO_S_FACTOR 1000000 
//tiempo que el ESP32 estara dormido (en segundos) 
#define TIME_TO_SLEEP 5

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.println(""); 
  Serial.println(""); 
  Serial.println("EXT1 Test");
  switch (wakeup_reason) { 
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break; 
    case 2  : {
      Serial.print("Wakeup caused by external signal using RTC_CNTL ");
      uint64_t a =(uint64_t)esp_sleep_get_ext1_wakeup_status();
      if (a==0x2000000) contador1++;
      else if (a==4) contador++;
      else if (a==0x800000000) contador2++;
      Serial.println();
      break;
    } 
    case 3  : Serial.println("Wakeup caused by timer"); contador_t++; break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break; 
    case 5  : Serial.println("Wakeup caused by ULP program"); break; 
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

void setup() { 
  // put your setup code here, to run once: 
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP); //pin 2 entrada con resistencia de pullup 
  pinMode(25, INPUT_PULLUP); //pin 25 entrada con resistencia de pullup 
  pinMode(35, INPUT_PULLUP); //pin 35 entrada con resistencia de pullup
  print_wakeup_reason();
  Serial.print(contador);
  Serial.print("    "); 
  Serial.print(contador_t);
  Serial.print("    ");
  Serial.print(contador1);
  Serial.print("    "); 
  Serial.println(contador2);
  delay (1000);
  //programacion del temporizador del RTC para que despierte 
  //al ESP32 al cabo del tiempo definido 
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); 
  //programación para usar varias entradas 
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_deep_sleep_start(); //duerme al ESP32 (modo SLEEP) 
}

void loop() { // put your main code here, to run repeatedly:
}
