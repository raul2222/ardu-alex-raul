/* Ejercicio 4: Sincronizacion entre ISR y tarea
Se interrumpe el micro al pulsar el boton A */

#define PIN_PULSADOR_A 39

//Crear el manejador para el semáforo
SemaphoreHandle_t xSemaphore= NULL; 

// Rutina de interrupción, llamada cuando se presiona el pulsador
void IRAM_ATTR pulsadorA_ISR() {

  // da el semáforo para que quede libre para la tarea pulsador
   xSemaphoreGiveFromISR(xSemaphore,NULL);

}

void task_pulsador(void* arg) {

   while(1) {
      // Esperar la notificación de la ISR
      xSemaphoreTake(xSemaphore, portMAX_DELAY);
      // Escribe mensaje
      Serial.println("Botón A pulsado\n");
      
   }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Sincronizacion ISR --> tarea: ");

  // Configurar interrupcion del pulsador A
  pinMode(PIN_PULSADOR_A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_PULSADOR_A), pulsadorA_ISR, FALLING);

   // Crear semaforo binario
   xSemaphore = xSemaphoreCreateBinary();

   // Crear la tarea task_pulsador
   xTaskCreate(task_pulsador, "task_pulsador", 2048, NULL, 1, NULL);
}

void loop() {}
