SemaphoreHandle_t xSemaphore1 = NULL;
SemaphoreHandle_t xSemaphore2 = NULL;
SemaphoreHandle_t xSemaphore3 = NULL;

void vATaskFunction1( void *pvParameters) { 
  for( ;; ) { 
    Serial.println("Tarea 1"); 
    xSemaphoreGive(xSemaphore1);
    vTaskDelay( 5000/portTICK_RATE_MS);
  } 
}
void vATaskFunction2( void *pvParameters) { 
  for( ;; ) { 
    xSemaphoreTake(xSemaphore1, portMAX_DELAY);
    Serial.println("Tarea 2"); 
    xSemaphoreGive(xSemaphore2);
  } 
}
void vATaskFunction3( void *pvParameters) { 
  for( ;; ) { 
    xSemaphoreTake(xSemaphore2, portMAX_DELAY);
    Serial.println("Tarea 3"); 
    xSemaphoreGive(xSemaphore3);
  } 
}
void vATaskFunction4( void *pvParameters) { 
  for( ;; ) { 
    xSemaphoreTake(xSemaphore3, portMAX_DELAY);
    Serial.println("Tarea 4"); 
  } 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  xSemaphore1 = xSemaphoreCreateBinary();
    xSemaphore2 = xSemaphoreCreateBinary();
    xSemaphore3 = xSemaphoreCreateBinary();
  xTaskCreate(    
    &vATaskFunction1, 
    "Tarea1", 
    1024, 
    NULL, 
    1, 
    NULL );
    //----------------------------
    xTaskCreate(    
    &vATaskFunction2, 
    "Tarea2", 
    1024, 
    NULL, 
    2, 
    NULL );
    //----------------------------
    xTaskCreate(    
    &vATaskFunction3, 
    "Tarea3", 
    1024, 
    NULL, 
    3, 
    NULL );
    //----------------------------
    xTaskCreate(    
    &vATaskFunction4, 
    "Tarea4", 
    1024, 
    NULL, 
    4, 
    NULL );
     
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
