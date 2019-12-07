#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;

void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare(20);
}

void loop() {

  if (scale.is_ready()) {
    //long reading = scale.get_value((10),0);
    Serial.print("HX711 reading: ");
    Serial.println(scale.get_value(100),0);
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1000);
  
}
