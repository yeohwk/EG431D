#include <Wire.h>
#include "rgb_lcd.h"
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS 1000
#define HR_THRES            100   // Heart rate threshold
#define BUZZER              2     // Buzzer connects to Digital Port D2

uint32_t tsLastReport = 0;
// Initialize LCD
rgb_lcd lcd;
// Initialize a Pulse Oximeter instance (beat detection, heart rate, SpO2)
PulseOximeter pox;
// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{
    pinMode(BUZZER, OUTPUT);
    Serial.begin(115200);
    lcd.begin(16, 2);                               // Init LCD
    lcd.setRGB(0, 255, 0);                          // Green LCD Back Light
    Serial.print("Initializing pulse oximeter..");
    lcd.print("Initializing POx.");
    // Initialize the PulseOximeter instance
    if (!pox.begin()) {
        Serial.println("FAILED");
        lcd.setRGB(255, 0, 0);                      // Red LCD Back Light
        lcd.print("FAILED");
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
    float hr;
    unsigned char spo2;
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        hr   = pox.getHeartRate();
        spo2 = pox.getSpO2();
        Serial.print("Heart rate:"); Serial.print(hr);
        Serial.print("bpm / SpO2:"); Serial.print(spo2);
        Serial.println("%");

        lcd.clear();
        lcd.setCursor(0, 0); lcd.print("HR  : "); lcd.print(hr);
        lcd.setCursor(0, 1); lcd.print("SPO2: "); lcd.print(spo2);

        if(hr >= HR_THRES) {digitalWrite(BUZZER, HIGH);}  
        else {digitalWrite(BUZZER, LOW);}

        tsLastReport = millis();
    }
}
