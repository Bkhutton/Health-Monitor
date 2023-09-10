#include <Arduino.h>


void setup() {
  Serial.begin(9600);
  initBMP280();
  initWifi();
  initMQTT();
  initDisplay();
}

void loop() {
  sendMQTT();
  updateScreen();
  delay(1000);
}
