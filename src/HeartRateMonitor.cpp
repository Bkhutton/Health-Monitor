#include <Arduino.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

// #define REPORTING_PERIOD_MS     1000

void updateHeartRateSensor();
void onBeatDetected();
void initHeartRateMonitor();

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;

void updateHeartRateSensor() {
    // Make sure to call update as fast as possible
    pox.update();
}

// Callback (registered below) fired when a pulse is detected
void onBeatDetected() {
    Serial.println("Beat!");
}

void initHeartRateMonitor() {
    Serial.print("Initializing pulse oximeter..");

    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // The default current for the IR LED is 50mA and it could be changed
    //   by uncommenting the following line. Check MAX30100_Registers.h for all the
    //   available options.
    // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback for the beat detection
    // pox.setOnBeatDetectedCallback(onBeatDetected);
}