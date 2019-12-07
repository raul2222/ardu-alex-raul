#ifndef SENSORDEDISTANCIA_YA_INCLUIDO
#define SENSORDEDISTANCIA_YA_INCLUIDO

class SensorDeDistancia {
  private:
  int EchoPin;
  int TriggerPin;
  int ledPin;
  int freq;
  int ledChannel;
  int resolution;
  int dutyCycle;
  bool encendido;

  public:
  SensorDeDistancia();
  void begin();
  int distancia();
  void toggleLuz();
  bool getEncendido();
  void setEncendido(bool toggle);
};

#endif
