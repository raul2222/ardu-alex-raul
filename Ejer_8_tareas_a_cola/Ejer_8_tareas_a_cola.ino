/* Ejercicio 8: Enviar datos de 3 tareas a una tarea receptora mediante una cola*/
#include "driver/uart.h"
#include "esp_intr_alloc.h"

#define TX0_pin  1
#define RX0_pin  3

#define BUF_SIZE (1024)
// numero de bytes recibidos para generar la interrupcion
#define UART_FULL_THRESH_DEFAULT  (1) 

#define TAM_COLA 20 /*20 mensajes*/

// Estructura para envio de datos
typedef struct{
  uint8_t id_sender;
  uint8_t data;
}type_msg;

// Declarar manejador de la cola
xQueueHandle xQueue; 

int msg1 = 0;
void escribe1(void *pvParameter)
{
    type_msg my_msg;
    my_msg.id_sender = 1;
    uint8_t cont_t1 = 0;
    while(1) {
    cont_t1++;
    my_msg.data = cont_t1;
    printf("Ejecuta Tarea escribe1\n");
    // Enviar datos a la cola
    xQueueSend(xQueue,&my_msg, portMAX_DELAY); 
    vTaskDelay(2000 / portTICK_RATE_MS);
    }
}
void escribe2(void *pvParameter)
{
    type_msg my_msg;
    my_msg.id_sender = 2;
    uint8_t cont_t2 = 0;
    while(1) {
    cont_t2++;
    my_msg.data = cont_t2;
    printf("Ejecuta Tarea escribe2\n");
    // Enviar datos a la cola
    xQueueSend(xQueue,&my_msg, portMAX_DELAY); 
     vTaskDelay(2000 / portTICK_RATE_MS);
 }
}

void escribe3(void *pvParameter)
{
  type_msg my_msg;
    my_msg.id_sender = 3;
    uint8_t cont_t3 = 0;
    while(1) {
    cont_t3++;
    my_msg.data = cont_t3;
    printf("Ejecuta Tarea escribe3\n");
    // Enviar datos a la cola
    xQueueSend(xQueue,&my_msg, portMAX_DELAY); 
    vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

void lee1(void *pvParameter)
{
   type_msg rx_msg;
   while(1) { 
      // Leer datos de la cola
      xQueueReceive(xQueue,&rx_msg,portMAX_DELAY);
      uart_write_bytes(UART_NUM_0, rx_msg, 1);
   }
}

void setup() {

    // Crear cola
    xQueue= xQueueCreate(TAM_COLA,sizeof(type_msg));

    // Crear tareas
    xTaskCreate(&lee1,     "lee1",     1024*10, NULL, 5, NULL);
    xTaskCreate(&escribe1, "escribe1", 1024, NULL, 1, NULL);
    xTaskCreate(&escribe2, "escribe2", 1024, NULL, 1, NULL);
    xTaskCreate(&escribe3, "escribe3", 1024, NULL, 1, NULL);

    // Inincializacion UARTS
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 120,
        .use_ref_tick = false
    };
  
  //uart_param_config(uart_port_t uart_num, const uart_config_t *uart_config);
  uart_param_config(UART_NUM_0, &uart_config);

  
  //uart_set_pin(uart_port_t uart_num, int tx_io_num, int rx_io_num, int rts_io_num, int cts_io_num);
  uart_set_pin(UART_NUM_0, TX0_pin, RX0_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  
  // uart_driver_install(uart_port_t uart_num, int rx_buffer_size, int tx_buffer_size, 
  uart_driver_install(UART_NUM_0, BUF_SIZE,0, 0, NULL, 0);

  
  uart_write_bytes(UART_NUM_0, "********************************\r\n", 34);
  uart_write_bytes(UART_NUM_0, "Comunicacion ISR->tarea con cola\r\n", 34);
  uart_write_bytes(UART_NUM_0, "********************************\r\n", 34);
}

void loop() {}
