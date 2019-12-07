void setup() 
{
  Serial.begin(9600);
  while (!Serial) {
    ; //esperar hasta que se conecte el puerto. Necesario para el puerto nativo USB solamente
  }

  delay(800);
  Serial.println("¡Hola!");
}

void loop() 
{
   // enviar datos solo cuando reciba datos:
        if (Serial.available() > 0) {
                // leer el siguiente int:
                int incomingInt;
                incomingInt = Serial.parseInt();
                
                // decir lo que ha leido:
                Serial.print("He recibido: ");
                Serial.println(incomingInt);
                incomingInt=incomingInt*2;
                delay(1000);
                Serial.print("El resultado es: ");
                Serial.println(incomingInt);
        }
}
