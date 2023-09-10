#include <Arduino.h>

/* MQTT Setup */
#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "home";
int        port     = 1883;
const char topic[]  = "temperature";
const char topic2[]  = "pressure";

const long interval = 8000;
unsigned long previousMillis = 0;


/* Dispaly Setup */
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R2, /* clock=*/ 18, /* data=*/ 23, /* cs=*/ 5, /* dc=*/ 16, /* reset=*/ 17);

/* BMP280 Setup */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C

void updateScreen() {
  u8g2.clearBuffer();					              // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	      // choose a suitable font
  u8g2.setCursor(0, 15);
  u8g2.print("Temp: ");
  u8g2.println(bmp.readTemperature());
  u8g2.setCursor(0, 30);
  u8g2.print("Pressure: ");
  u8g2.println(bmp.readPressure());
  u8g2.sendBuffer();					              // transfer internal memory to the display
}

void sendMQTT() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(bmp.readTemperature());

    Serial.print("Sending message to topic: ");
    Serial.println(topic2);
    Serial.println(bmp.readPressure());

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(bmp.readTemperature());
    mqttClient.endMessage();

    mqttClient.beginMessage(topic2);
    mqttClient.print(bmp.readPressure());
    mqttClient.endMessage();

    Serial.println();
  }
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

void initBMP280() {
  Serial.println(F("Initializing BMP280"));
  unsigned bmpStatus;
  bmpStatus = bmp.begin();
  if (!bmpStatus) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16); 
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void initDisplay() {
  u8g2.begin();
}

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
