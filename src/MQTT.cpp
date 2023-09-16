/* MQTT Setup */
#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include <BMP280.h>
#include <HeartRateMonitor.h>
#include "wifi_secrets.h"

void initWifi();
void initMQTT();
void sendMQTT();
void updateMQTT();

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "home";
int        port     = 1883;
const char topic[]  = "temperature";
const char topic2[]  = "pressure";
const char topic3[]  = "heartRate";
const char topic4[]  = "spO2";


void initWifi() {
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println("You're connected to the network");
  Serial.println();
}

void initMQTT() {
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  mqttClient.setUsernamePassword("admin", "password");

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void updateMQTT() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();
}

void sendMQTT() {
  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage(topic);
  mqttClient.print(bmp.readTemperature());
  mqttClient.endMessage();

  mqttClient.beginMessage(topic2);
  mqttClient.print(bmp.readPressure());
  mqttClient.endMessage();

  mqttClient.beginMessage(topic3);
  mqttClient.print(pox.getHeartRate());
  mqttClient.endMessage();

  mqttClient.beginMessage(topic4);
  mqttClient.print(pox.getSpO2());
  mqttClient.endMessage();
}