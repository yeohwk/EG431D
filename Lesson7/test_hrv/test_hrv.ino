#include <Wire.h>
#include <arduinoFFT.h> // FFT library
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000
#define FFT_SIZE 64 // Number of FFT points (256)

PulseOximeter pox;
unsigned long tsLastReport = 0;
double rrIntervals[FFT_SIZE]; // Store RR intervals
int rrIndex = 0; // Current index for storing RR intervals

// FFT configuration

double vReal[FFT_SIZE];
double vImag[FFT_SIZE];


//arduinoFFT FFT = arduinoFFT();
//ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, FFT_SIZE, 100);

// Callback function to collect RR intervals
void onBeatDetected() {
    static unsigned long lastBeatTime = 0;
    unsigned long currentBeatTime = millis();

    if (lastBeatTime > 0) {
        double rrInterval = (currentBeatTime - lastBeatTime) / 1000.0; // Convert to seconds
        if (rrIndex < FFT_SIZE) {
            rrIntervals[rrIndex++] = rrInterval;
        }
    }

    lastBeatTime = currentBeatTime;
}
/*
void processFFT() {
    // Zero-pad if not enough RR intervals
    for (int i = 0; i < FFT_SIZE; i++) {
        vReal[i] = (i < rrIndex) ? rrIntervals[i] : 0;
        vImag[i] = 0; // Imaginary part is zero
    }

    // Perform FFT
    FFT.windowing(vReal, FFT_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD, NULL, false);
    FFT.compute(vReal, vImag, FFT_SIZE, FFT_FORWARD);
    FFT.complexToMagnitude(vReal, vImag, FFT_SIZE);

    // Analyze frequency spectrum
    double samplingRate = rrIndex / (rrIntervals[rrIndex - 1] * 0.001); // Derive sampling rate
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
    Serial.println("Frequency Domain Analysis:");
    for (int i = 1; i < (FFT_SIZE / 2); i++) {
        double frequency = (i * samplingRate) / FFT_SIZE;
        Serial.print("Frequency: ");
        Serial.print(frequency, 3);
        Serial.print(" Hz, Magnitude: ");
        Serial.println(vReal[i], 3);
    }

    Serial.print("Peak Frequency: ");
    Serial.print(peakFrequency, 3);
    Serial.println(" Hz");
}
*/

void setup() {
    Serial.begin(115200);
    Serial.print("Initializing pulse oximeter...");
//    while(1);

    if (!pox.begin()) {
        Serial.println("FAILED");
        while (1)
        {
          Serial.println("FAILED");
        }
    } else {
        Serial.println("SUCCESS");
    }

    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
//    pox.update();

    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        if (rrIndex >= FFT_SIZE) {
//            processFFT();
            rrIndex = 0; // Reset RR interval collection
        }
        tsLastReport = millis();
    }
}
