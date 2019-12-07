//#include <esp_task_wdt.h>

void vATaskFunction1( void *pvParameters) { for( ;; ) { Serial.println("Tarea 1"); vTaskDelay( 5000/portTICK_RATE_MS);} }
void vATaskFunction2( void *pvParameters) { for( ;; ) { Serial.println("Tarea 2"); vTaskDelay( 5000/portTICK_RATE_MS);} }
void vATaskFunction3( void *pvParameters) { for( ;; ) { Serial.println("Tarea 3"); vTaskDelay( 5000/portTICK_RATE_MS);} }
void vATaskFunction4( void *pvParameters) { for( ;; ) { Serial.println("Tarea 4"); vTaskDelay( 5000/portTICK_RATE_MS);} }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
