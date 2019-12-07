const int EchoPin = 36;
const int TriggerPin = 26;

void loop() {   
  if (Serial.available() > 0) {     
    char command = (char) Serial.read();     
    switch (command) {      
      case 'H':        
      Serial.println("Hola Mundo");        
      break;      
      case 'D':        
      Serial.println(distancia(TriggerPin, EchoPin));        
      break;     
    }   
  }   
  Serial.flush(); 
}
