/* Control del sensor TC 72 mediante SPI */

#include <M5Stack.h>
#include <SPI.h>

#define DEV1_CS 21        

// Funciones a realizar en los ejercicios
//void write_1byte(SPISettings SPI_settings,uint8_t dev_cs_pin,uint8_t write_addr, uint8_t data);
//uint8_t read_1byte(SPISettings SPI_settings,uint8_t dev_cs_pin,uint8_t read_addr);
//uint16_t read_2bytes(SPISettings SPI_settings,uint8_t dev_cs_pin,uint8_t read_addr);
//uint8_t * read_2bytes_p(SPISettings SPI_settings,uint8_t dev_cs_pin,uint8_t read_addr);

void write_1byte(uint8_t dev_cs_pin,uint8_t write_addr, uint8_t data);
uint8_t read_1byte(uint8_t dev_cs_pin,uint8_t read_addr);
uint16_t read_2bytes(uint8_t dev_cs_pin,uint8_t read_addr);
uint8_t * read_2bytes_p(uint8_t dev_cs_pin,uint8_t read_addr);

// Estructura para almacenar los comandos A y D
typedef struct{  
      uint8_t addr; // A: Address
      uint8_t data_val; // D: Data
} comando;

// Funcion para leer los comandos A y D desde el monitor serie
comando lee_comando(void);


//para poner a 0 el bit 7 -> 0x7F & dato
//para poner a 1 el bit 7 -> 0x80 | dato


void setup() {
  M5.begin();
  pinMode (DEV1_CS, OUTPUT);
  digitalWrite(DEV1_CS, LOW);

  Serial.begin(115200);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("Test lectura SPI\r\n");
  Serial.println("  ");
  Serial.println("------------------------------------");
  Serial.println("Prueba de lectura en bus SPI");
  Serial.println("------------------------------------");

  //SPI.begin();
  Serial.println("Introduce un comando (A: Address; D: Data) ");
}

void loop() {
 // Lee comandos y datos
   comando cmd;
   cmd = lee_comando();
   uint8_t addr = cmd.addr;
   uint8_t data_val = cmd.data_val;

 // Rutinas pulsadores   
  M5.BtnA.read();
  if(M5.BtnA.wasPressed()) {
	// Leer el byte de la direccion addr y almacenarlo en data8
	uint8_t data8 = 0; 
    data8 = read_1byte(DEV1_CS, addr);
    Serial.print("LECTURA DEV1 = Dir 0x");
    Serial.print(addr,HEX);
    Serial.print(" - Dato 0x");
    Serial.println(data8,HEX);
  }

   M5.BtnB.read();
   if(M5.BtnB.wasPressed()) {
      // Escribir el dato en data_val en la direccion addr
      write_1byte(DEV1_CS, addr, data_val);
      Serial.print("ESCRITURA DEV1 = Dir 0x");
      Serial.print(addr,HEX);
      Serial.print(" - Dato 0x");
      Serial.println(data_val,HEX);
   }

  M5.BtnC.read();
  if(M5.BtnC.wasPressed()) {
    // Leer la palabra de 2 bytes de la direccion addr y almacenarla en data16
	uint16_t data16 = 0;
   
    Serial.print("LECTURA DEV1 = Dir 0x");
    Serial.print(addr,HEX);
    Serial.print(" - Dato 0x");
    Serial.println(data16,HEX);
  }
}


    
void write_1byte(uint8_t dev_cs_pin,uint8_t write_addr, uint8_t data) {
  // Escribir la dirección del registro que se quiere escribir 
  // y despuéss escribir su valor    

  //SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  digitalWrite(dev_cs_pin, 1);
  SPI.transfer(0x80 | read_addr);
  SPI.transfer(data)
  //dato = SPI.transfer(read_addr);
  digitalWrite(dev_cs_pin, 0);
  SPI.endTransaction();
  
  
  }

uint8_t read_1byte(uint8_t dev_cs_pin,uint8_t read_addr) {
  // Escribir la dirección del registro que se quiere leer 
  // y despuéss se lee su valor    
  uint8_t dato = 0;
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  digitalWrite(dev_cs_pin, 1);
  SPI.transfer(0x7F & read_addr);
  dato = SPI.transfer(read_addr);
  digitalWrite(dev_cs_pin, 0);
  SPI.endTransaction();
  
  return dato;
  } 
 
 uint16_t read_2bytes(uint8_t dev_cs_pin,uint8_t read_addr) {
  // Escribir la dirección del registro que se quiere leer 
  // y despuéss se lee su valor y el de la dirección anterior a la dada
  // y se presentan concatenados    
  uint16_t dato = 0;

  
  
  
  
  return dato;
  }


  
////////////////////////////////////////////////////////////////////////////////
// Funcion para leer los comandos A (Address) y D (Data) desde el monitor serie
////////////////////////////////////////////////////////////////////////////////
comando lee_comando(void){
  char dato = 'A';
  char num = 0;
  uint16_t dig;
  static  comando cmd;
  
  uint8_t num_bytes = Serial.available();
   if (num_bytes>0) // Lee comando "Dir" o "Dato"
   {
     dato = Serial.read();
     if ((dato == 'A') || dato == 'D')
     {
        Serial.print("Comando recibido : ");
        Serial.println(dato);
        num_bytes = 0;
        num = 0;
        dig = 0;
        Serial.println("Introduce el valor");
  
        while (num_bytes == 0) // Lee número
        {
          num_bytes = Serial.available();
         }
        uint8_t cont_dig = 0;
         while (num_bytes > 0)
         {
            num_bytes -= 1;
            num = Serial.read();
            num -= '0';
            dig += num*pow(10,num_bytes);      
         }
         Serial.print(dato);
         Serial.print(" = ");
         Serial.println(dig);
         
         Serial.println("Introduce un comando (A: Address; D: Data) ");
         
         if (dato == 'A')
         {
            cmd.addr = (uint8_t)dig;
            if (dig >3){
              Serial.println("ERROR: A debe ser menor que 4");
            }
         }
         if (dato == 'D')
         {
            cmd.data_val = (uint8_t)dig;
            if (dig >255){
              Serial.println("ERROR: D debe ser menor que 256");
            }
         } 
     }
     else
     {
        Serial.println("Comando NO válido: debe ser A o D");
     } 
    }
	
return cmd;
}
