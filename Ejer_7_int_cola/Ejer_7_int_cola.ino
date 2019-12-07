/* Ejercicio 7: Enviar datos de una ISR a una tarea mediante una cola*/

#include <M5Stack.h>
#include "driver/uart.h"
#include "esp_intr_alloc.h"

#define TX0_pin  1
#define RX0_pin  3

#define BUF_SIZE (1024)
// numero de bytes recibidos para generar la interrupcion
#define UART_FULL_THRESH_DEFAULT  (1) 

#define TAM_COLA 10 /*num mensajes*/
#define TAM_MSG 1 /*num caracteres por mensaje*/

// 1. Declaracion manejador de la cola //////////////////////////////////
xQueueHandle xQueue; 

//Declaracion manejador interrupcion
static intr_handle_t handle_console;

// Rutina de atencion a la interrupcion de la UART
static void  IRAM_ATTR uart_intr_handle(void *arg)
{
  char  rxbuf[TAM_COLA];
  uint8_t rx_fifo_len;
  uint8_t i = 0;
 
  // Leer el numero de bytes del buffer de la UART
  rx_fifo_len = UART0.status.rxfifo_cnt; // read number of bytes in UART buffer
  
  // Leer los bytes del buffer
  while(rx_fifo_len){
    // Leer los bytes recibidos
    rxbuf[i++] = UART0.fifo.rw_byte;

    rx_fifo_len--;
  }
    // 3. Enviar los bytes a la cola 
    xQueueSendFromISR(xQueue, &rxbuf, pdFALSE); 
    
   // Limpiar el bit de status de la interrupcion de la UART
   uart_clear_intr_status(UART_NUM_0, UART_RXFIFO_FULL_INT_CLR);
}

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("\n\n");
  M5.Lcd.printf("  Comunicacion\r\n"); 
  M5.Lcd.printf("\r\n"); 
  M5.Lcd.printf("   ISR->tarea\r\n"); 
  M5.Lcd.printf("\r\n");
  M5.Lcd.printf("    con cola\r\n");
  
  // Deshabilita la UART0 para configurarla con funciones idf
  Serial.end(); 

  // 2. Crear  cola //////////////////////////////////////////////
  xQueue= xQueueCreate(TAM_COLA,TAM_MSG);
  
  // Creacion de tarea
  xTaskCreate(&lee_cola,"lee_cola",1024, NULL, 5, NULL);
  
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

  // Liberar el manejador de la ISR UART
  uart_isr_free(UART_NUM_0);
  
  // Registrar la ISR de la UART
  uart_isr_register(UART_NUM_0,uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LEVEL1, &handle_console);
  
  // Configurar la interrupcion UART 
    uart_intr_config_t uart0_intr;
    uart0_intr.intr_enable_mask = UART_RXFIFO_FULL_INT_ENA_M ;
    uart0_intr.rxfifo_full_thresh = UART_FULL_THRESH_DEFAULT;
    
  uart_intr_config(UART_NUM_0, &uart0_intr);

  // Habilitar la máscara de interrupciones UART
  uart_enable_intr_mask(UART_NUM_0, UART_RXFIFO_FULL_INT_ENA );//| UART_RXFIFO_TOUT_INT_ENA);
  
}

void lee_cola(void *pvParameter)
{
   int i;
   char Rx[TAM_COLA];
   while(1) { 
      // 4.a. Leer los datos de la cola
      xQueueReceive(xQueue, &Rx, portMAX_DELAY);
      // 4.b. Escribir los datos en el monitor serie
      uart_write_bytes(UART_NUM_0, Rx, 1);
   }
}

  void loop() {}
