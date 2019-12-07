#define LENGTH_QUEUE 10
#define NUM_DATA_TO_QUEUE 12
#define FIFO_ticks 1

QueueHandle_t queueBack;
QueueHandle_t queueFront;
 
void setup() {
 
  Serial.begin(115200);
 
  queueBack = xQueueCreate( LENGTH_QUEUE, sizeof( int ) );
  queueFront = xQueueCreate( LENGTH_QUEUE, sizeof( int ) );
 
  if(queueBack == NULL || queueFront ==NULL){
    Serial.println("Error creating one of the queues");
  }
 
}
static int i_val = 0;
void loop() {
 
  if(queueBack == NULL || queueFront == NULL )return;
 
  for(int i = i_val; i<i_val+NUM_DATA_TO_QUEUE; i++){
    
    if(xQueueSendToBack(queueBack, &i, FIFO_ticks)!=pdTRUE)
    {
      Serial.println("FIFO full");
    };
    if (xQueueSendToFront(queueFront, &i, FIFO_ticks)!=pdTRUE)
    {
      Serial.println("LIFO full");
    };
  }
  i_val = i_val + NUM_DATA_TO_QUEUE;
  int element;
 
  Serial.println("Back queue:");
 
  for(int i = 0; i<NUM_DATA_TO_QUEUE; i++){
    if (xQueueReceive(queueBack, &element, FIFO_ticks)!=pdTRUE)
    {
      Serial.println("FIFO empty");
    };
    Serial.print(element);
    Serial.print("|");
  }
Serial.println();
  Serial.println("Front queue:");
 
  for(int i = 0; i<NUM_DATA_TO_QUEUE; i++){
    if (xQueueReceive(queueFront, &element, FIFO_ticks)!=pdTRUE)
    {
      Serial.println("LIFO empty");
    };
    Serial.print(element);
    Serial.print("|");
  }
Serial.println();
  Serial.println("--------------");
  delay(5000);
}
