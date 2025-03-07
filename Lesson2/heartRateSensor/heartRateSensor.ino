#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS 1000

// Initialize a Pulse Oximeter instance (beat detection, heart rate, SpO2)
PulseOximeter pox;
uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{
    Serial.begin(115200);
    Serial.print("Initializing pulse oximeter..");
    // Initialize the PulseOximeter instance
    if (!pox.begin()) {
        Serial.println("FAILED");
        while(1);
    } 
    else {
        Serial.println("SUCCESS");
    }
    // The default current for the IR LED is 50mA and it can be modified
    // by editing the following line. Check MAX30100_Registers.h for all
    // the available options.
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        tsLastReport = millis();
    }
}

