#include <Arduino.h>
#include <BMP280.h>
#include <HeartRateMonitor.h> 
#include <MQTT.h>
#include <Display.h>


const long interval = 8000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  initWifi();
  initMQTT();
  initDisplay();
  initBMP280();
  initHeartRateMonitor();
}

void loop() {
  updateHeartRateSensor();
  updateMQTT();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendMQTT();
  }

  updateScreen(); 
}