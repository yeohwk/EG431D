#include <Wire.h>
#include "rgb_lcd.h"
#include "MAX30100_PulseOximeter.h"
#include <arduinoFFT.h>         // FFT library

#define REPORTING_PERIOD_MS 1000
#define FFT_SIZE 64             // Number of FFT points

// Initialize LCD
rgb_lcd lcd;

// Initialize a MAX30100 Pulse Oximeter instance (beat detection, heart rate, SpO2)
// Default MAX30100 sampling frequency: 100Hz
PulseOximeter pox;

bool hbeatLED = false;
uint32_t tsLastReport = 0;
double rrIntervals[FFT_SIZE];   // Store RR intervals
int rrIndex = 0;                // Current index for storing RR intervals

// FFT configuration
double vReal[FFT_SIZE];
double vImag[FFT_SIZE];
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, FFT_SIZE, 100.0);

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    static unsigned long lastBeatTime = 0;
    unsigned long currentBeatTime = millis();

    if (lastBeatTime > 0) {
        double rrInterval = (currentBeatTime - lastBeatTime) / 1000.0; // Convert to seconds
        if (rrIndex < FFT_SIZE) {
            rrIntervals[rrIndex++] = rrInterval;
        }
    }

    lastBeatTime = currentBeatTime;
    hbeatLED = !hbeatLED;
    digitalWrite(LED_BUILTIN, hbeatLED);
}

void processFFT() {
    double averageInterval = 0;
    // Zero-pad if not enough RR intervals
    for (int i = 0; i < FFT_SIZE; i++) {
        vReal[i] = (i < rrIndex) ? rrIntervals[i] : 0;
        vImag[i] = 0; // Imaginary part is zero

        averageInterval = averageInterval + rrIntervals[i];
    }
    averageInterval = averageInterval / (double) FFT_SIZE;

    // Perform FFT
    FFT.windowing(vReal, FFT_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD, NULL, false);
    FFT.compute(vReal, vImag, FFT_SIZE, FFT_FORWARD);
    FFT.complexToMagnitude(vReal, vImag, FFT_SIZE);

  // Analyze frequency spectrum
  //double samplingRate = 1.0; // Derive sampling rate
    double samplingRate = (1 / averageInterval);    

    double peakFrequency = 0;
    double maxMagnitude = 0;
    for (int i = 1; i < (FFT_SIZE / 2); i++) {
        double frequency = (i * samplingRate) / FFT_SIZE;
        if (vReal[i] > maxMagnitude) {
            maxMagnitude = vReal[i];
            peakFrequency = frequency;
        }
    }

    // Print frequency domain results
//    Serial.println("Frequency Domain Analysis:");
    for (int i = 1; i < (FFT_SIZE / 2); i++) {
        double frequency = (i * samplingRate) / FFT_SIZE;
//        Serial.print("Frequency: ");
//        Serial.print(frequency, 3);
//        Serial.print(" Hz, Magnitude: ");
        Serial.println(vReal[i], 3);
    }

//    Serial.print("Peak Frequency: ");
//    Serial.print(peakFrequency, 3);
//    Serial.println(" Hz");
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    lcd.begin(16, 2);                               // Init LCD
    lcd.setRGB(0, 255, 0);                          // Green LCD Back Light
    lcd.print("Initializing POx.");

    // Initialize the PulseOximeter instance
    if (!pox.begin()) {
        lcd.setRGB(255, 0, 0);                      // Red LCD Back Light
        lcd.print("FAILED");      
        while(1);
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
    // Make sure to call update as fast as possible
    pox.update();
    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        hr   = pox.getHeartRate();
        spo2 = pox.getSpO2();

        lcd.clear();
        lcd.setCursor(0, 0); lcd.print("H:"); lcd.print(hr); lcd.print(" S:"); lcd.print(spo2); lcd.print("%");
        lcd.setCursor(0, 1); lcd.print("["); lcd.print(rrIndex); lcd.print("]");

        if (rrIndex >= FFT_SIZE) {
            processFFT();
            rrIndex = 0; // Reset RR interval collection
        }
        tsLastReport = millis();
    }
}

