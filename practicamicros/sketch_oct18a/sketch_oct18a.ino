#include <M5Stack.h>
#include "driver\timer.h"
#include "driver/uart.h"
#include "esp_intr_alloc.h"
#define Pin 36
#define TX0_pin  1
#define RX0_pin  3
#define BUF_SIZE (1024)
#define K 0.000887573964497

hw_timer_t * timer0 = NULL; 
volatile int Flag_ISR_Timer0 = 0;
volatile int flag_LCD = 0;

bool screen1 = false;

int average = 0;
int maximo = 0;
int minimo = 32000;
typedef struct
 {
     int raw_value;
 }  record;
 
record lecturas[200];

char buf[7];

int i = 0;

#define UART_EMPTY_THRESH_DEFAULT  (0)
#define UART_FULL_THRESH_DEFAULT  (1) // numero de bytes recibidos para generar la interrupcion
#define UART_TOUT_THRESH_DEFAULT   (0)


// Manejador para la ISR
static intr_handle_t handle_console;


 // Buffer de recepcion
volatile uint8_t  rxbuf[BUF_SIZE];
// Flag para activar rutina asociada a la ISR
volatile uint8_t Flag_uart_int = 0;

// Rutina de atencion a la interrupcion de la UART
static void  IRAM_ATTR ISR_UART(void *arg)
{
  uint8_t rx_fifo_len;
  uint8_t i = 0;
  
  // Leer el numero de bytes del buffer de la UART
  rx_fifo_len = UART0.status.rxfifo_cnt; 
  
  // Leer los bytes del buffer
  while(rx_fifo_len){
   rxbuf[i++] = UART0.fifo.rw_byte; // read all bytes
   rx_fifo_len--;
   Flag_uart_int = 1;
  }

 // Limpiar el bit de status de la interrupcion de la UART
 uart_clear_intr_status(UART_NUM_0, UART_RXFIFO_FULL_INT_CLR);
 
}

void IRAM_ATTR ISR_Timer0(){
  lecturas[i].raw_value = analogRead(Pin);
  //Serial.println(lecturas[i].raw_value);
  i++;
  Flag_ISR_Timer0 = 1;
}

void mostrarPorSerial(){
  //uart_write_bytes(UART_NUM_0, "entra", 5);
  int suma = 0;
  for(int i = 0; i<200; i++){
    suma = suma + lecturas[i].raw_value;
  }
  int media = suma/200;
  //Serial.print("RAW: ");Serial.print(lecturas[i].raw_value);Serial.print(" Voltaje: ");Serial.println(lecturas[i].raw_value*K);
  itoa(lecturas[i].raw_value, buf, 10);
  uart_write_bytes(UART_NUM_0, "Raw: ", 5);
  uart_write_bytes(UART_NUM_0,(const char*)buf,strlen(buf));
  uart_write_bytes(UART_NUM_0,"\r\n",4);
  itoa(lecturas[i].raw_value*K, buf, 10);
  uart_write_bytes(UART_NUM_0, "Voltaje: ", 9);
  snprintf(buf,sizeof(buf),"%f",lecturas[i].raw_value*K);
  uart_write_bytes(UART_NUM_0,(const char*)buf,strlen(buf));
  uart_write_bytes(UART_NUM_0,"\r\n",4);
  itoa(maximo*K, buf, 10);
  uart_write_bytes(UART_NUM_0, "Maximo: ", 8);
  snprintf(buf,sizeof(buf),"%f",maximo*K);
  uart_write_bytes(UART_NUM_0,(const char*)buf,strlen(buf));
  uart_write_bytes(UART_NUM_0,"\r\n",4);
  itoa(minimo*K, buf, 10);
  uart_write_bytes(UART_NUM_0, "Minimo: ", 8);
  snprintf(buf,sizeof(buf),"%f",minimo*K);
  uart_write_bytes(UART_NUM_0,(const char*)buf,strlen(buf));
  uart_write_bytes(UART_NUM_0,"\r\n",4);
  itoa(average*K, buf, 10);
  uart_write_bytes(UART_NUM_0, "Average: ", 9);
  snprintf(buf,sizeof(buf),"%f",average*K);
  uart_write_bytes(UART_NUM_0,(const char*)buf,strlen(buf));
  uart_write_bytes(UART_NUM_0,"\r\n",4);
  //Serial.print("MAX: ");Serial.print(maximo*K);Serial.print(" MIN: ");Serial.print(minimo*K);Serial.print(" Average: ");Serial.println(average*K);
 }

void mostarPorPantalla(){
  //Serial.println("entra");
  /*for(int i = 0; i <= 199; i++){
    Serial.println(lecturas[i].raw_value*K);
  }*/
   M5.Lcd.clear(BLACK);
   M5.Lcd.setCursor(0,16); 
   M5.Lcd.printf("RAW:  %u", lecturas[i].raw_value);
   M5.Lcd.setCursor(0,32); 
   M5.Lcd.printf("VOLTAJE:  %f", lecturas[i].raw_value*K);

   M5.Lcd.setCursor(0,48); 
   M5.Lcd.printf("MAXIMO:  %f", maximo*K);
   M5.Lcd.setCursor(0,64); 
   M5.Lcd.printf("MINIMO:  %f", minimo*K);
   M5.Lcd.setCursor(0,80); 
   M5.Lcd.printf("AVERAGE:  %f", average*K);
   M5.update();
}

void procesarMaxMinAverage(){
  minimo = 65500;
  maximo = 0;
  average = 0;
  for (int i = 0; i <= 199; i++){
    if (lecturas[i].raw_value > maximo) {maximo = lecturas[i].raw_value;}
    if (lecturas[i].raw_value < minimo && lecturas[i].raw_value != 0) {minimo = lecturas[i].raw_value;}
    average = average + lecturas[i].raw_value;
  }
  //itoa(minimo, buf, 10);
  //uart_write_bytes(UART_NUM_0, buf, 10);
  average = average / 200;
  
}

void setup() {
  // put your setup code here, to run once:
  M5.begin(true, false, true);
  pinMode(Pin, INPUT_PULLUP);
  Serial.begin(115200);
  delay(1790);  Serial.println("Inicio");
  delay(1790);  Serial.println("Inicio");
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLACK);  M5.Lcd.setTextColor(WHITE);

  timer0 = timerBegin(0,80,true);
  timerAttachInterrupt(timer0,&ISR_Timer0,true);
  timerAlarmWrite(timer0,10000,true);
  timerAlarmEnable(timer0);

  Serial.end();

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

  
  uart_write_bytes(UART_NUM_0, "***************\r\n", 17);
  uart_write_bytes(UART_NUM_0, "Interrup. UARTs\r\n", 17);
  uart_write_bytes(UART_NUM_0, "***************\r\n", 17);


  // Liberar el manejador de la ISR UART
  uart_isr_free(UART_NUM_0);
  
  // Registrar la ISR de la UART
  uart_isr_register(UART_NUM_0,ISR_UART, NULL, ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LEVEL1, &handle_console);
  
  // Configurar la interrupcion UART
    uart_intr_config_t uart0_intr;
    uart0_intr.intr_enable_mask = UART_RXFIFO_FULL_INT_ENA_M ;
    uart0_intr.rxfifo_full_thresh = UART_FULL_THRESH_DEFAULT;
    uart0_intr.rx_timeout_thresh = UART_TOUT_THRESH_DEFAULT;
    //uart0_intr.txfifo_empty_intr_thresh = UART_EMPTY_THRESH_DEFAULT;
   
  uart_intr_config(UART_NUM_0, &uart0_intr);

  // Habilitar la máscara de interrupciones
  uart_enable_intr_mask(UART_NUM_0, UART_RXFIFO_FULL_INT_ENA );
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Flag_ISR_Timer0){
    if  (i == 199){
      procesarMaxMinAverage();
      screen1 = true;
      //delay(1000);
    }
    if(i==99 || i==199){
      if(screen1 == true){
        if(i==199){
          i = 0;
        }
        flag_LCD = 1;
      }
    }
    Flag_ISR_Timer0 = 0;
  }
  
  if(flag_LCD){
    mostarPorPantalla();
    flag_LCD = 0;
  }
  if(Flag_uart_int){
    //uart_write_bytes(UART_NUM_0, "entra", 5);
    for(int i = 0; i<BUF_SIZE; i++){
      if(rxbuf[i] == 'a'){
        mostrarPorSerial();
      }
    }
    Flag_uart_int = 0;
  }
  /*if(Serial.available()){
    char command = (char) Serial.read();
    if(command == 'a'){
      mostrarPorSerial();
    }
  }*/
}
