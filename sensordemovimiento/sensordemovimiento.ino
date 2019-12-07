const int EchoPin = 36; 
const int TriggerPin = 26;  
// the number of the LED pin
const int ledPin = 3;  // 16 corresponds to GPIO16
// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int dutyCycle = 0;

void setup() {    
  Serial.begin(9600);    
  pinMode(TriggerPin, OUTPUT);    
  pinMode(EchoPin, INPUT); 
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  delay(2500);
  //Serial.println("Inicio");
  //Serial.println(distancia(TriggerPin, EchoPin));  
}   
  
void loop() {    
//  Serial.print("Distancia: ");    
//  Serial.println(distancia(TriggerPin, EchoPin));    
//  delay(1000); 
  /*if (Serial.available()) {  
     //Serial.println("LLEGA");   
    char command = (char) Serial.read();     
    switch (command) {      
      case 'H':        
      Serial.println("Hola Mundo");        
      break;      
      case 'D':        
      Serial.println(distancia(TriggerPin, EchoPin));        
      break;     
    } 
    //Serial.println(command);  
  }   */
  int dis = distancia(TriggerPin, EchoPin);
  Serial.println(dis);
  if(dis<30){
    if(dutyCycle == 0 || dutyCycle == -1){
       // increase the LED brightness
  for(dutyCycle; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
    } else if(dutyCycle > 0 && dutyCycle <= 256){
  // decrease the LED brightness
  for(dutyCycle; dutyCycle >= 0; dutyCycle=dutyCycle-1){
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);   
    delay(15);
  }
    }
  }
  Serial.println(dutyCycle);
  delay(500);
} 
 
int distancia(int TriggerPin, int EchoPin) {    
  long duracion, distanciaCm;    
  digitalWrite(TriggerPin, LOW); //nos aseguramos señal baja al principio    
  delayMicroseconds(4);    
  digitalWrite(TriggerPin, HIGH); //generamos pulso de 10us    
  delayMicroseconds(10);    
  digitalWrite(TriggerPin, LOW);    
  duracion = pulseIn(EchoPin, HIGH);  //medimos el tiempo del pulso
  //Serial.println(duracion);    
  distanciaCm = duracion * 10 / 292 / 2;   //convertimos a distancia    
  return distanciaCm; 
}
