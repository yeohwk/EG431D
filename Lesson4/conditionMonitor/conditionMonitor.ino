#include <math.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Seeed_AMG8833_driver.h"
#include "ADXL345.h"

#define SOUNDSENSOR       A2    // Arduino A2 analog pin  connects to Sound Sensor 
#define SOUNDSAMPLEWINDOW 50    // 50ms sound sampling window
#define SOUND_THRESHOLD   20    // Sound Level Threshold (raw)
#define TEMP_THRESHOLD    40    // Max Temperature Threshold (degree C)
#define VIBR_THRESHOLD    1.9   // Vibration Threshold (g)

rgb_lcd lcd;                    // Initialize LCD instance
AMG8833 tsa(0x69);              // Initialize AMG8833 instance of the  AMG8833 TSA library
ADXL345 adxl;                   // Initialize ADXL345 instance of the ADXL345 Accelerometer library       

unsigned int getSoundLevel(char analogPort)
{
    unsigned long startMillis= millis();               // Start of sample window
    unsigned int soundSample;                          // Sound Sample 
    unsigned int soundLevel = 0;                       // peak-to-peak level
    unsigned int soundSignalMax = 0;
    unsigned int soundSignalMin = 1024;
    while (millis() - startMillis < SOUNDSAMPLEWINDOW)
    {
        soundSample = analogRead(analogPort);
        if (soundSample < 1024)  // toss out spurious readings
        {
            if (soundSample > soundSignalMax)
            {
                soundSignalMax = soundSample;          // save just the max levels
            }
            else if (soundSample < soundSignalMin)
            {
                soundSignalMin = soundSample;          // save just the min levels
            }
        }
    }
    soundLevel = soundSignalMax - soundSignalMin;      // max - min = peak-peak amplitude
    return soundLevel;       
}

void adxlInitialize()
{
    adxl.powerOn();

    // Set activity/inactivity thresholds (0-255)
    adxl.setActivityThreshold(75);    // 62.5mg per increment
    adxl.setInactivityThreshold(75);  // 62.5mg per increment
    adxl.setTimeInactivity(10);       // Set inactive timeout

    // Monitoring of activity movement on these axes -> 1 = on; 0 = off
    adxl.setActivityX(1);
    adxl.setActivityY(1);
    adxl.setActivityZ(1);

    // Monitoring of inactivity movement on these axes -> 1 = on; 0 = off
    adxl.setInactivityX(1);
    adxl.setInactivityY(1);
    adxl.setInactivityZ(1);

    // Monitoring of tap movement on these axes -> 1 = on; 0 = off
    adxl.setTapDetectionOnX(0);
    adxl.setTapDetectionOnY(0);
    adxl.setTapDetectionOnZ(1);

    // Set values for tap, and double-tap (0-255)
    adxl.setTapThreshold(50);     //62.5mg per increment
    adxl.setTapDuration(15);      //625us per increment
    adxl.setDoubleTapLatency(80); //1.25ms per increment
    adxl.setDoubleTapWindow(200); //1.25ms per increment

    // Set freefall values (0-255)
    adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
    adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment

    // Setting all interrupts to take place on Int pin 1
    // Issue (bug) with Int pin 2 -> unable to reset it
    adxl.setInterruptMapping(ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN);

    // Register interrupt actions - 1 == on; 0 == off
    adxl.setInterrupt(ADXL345_INT_SINGLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_DOUBLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_FREE_FALL_BIT,  1);
    adxl.setInterrupt(ADXL345_INT_ACTIVITY_BIT,   1);
    adxl.setInterrupt(ADXL345_INT_INACTIVITY_BIT, 1);  
} 

float getMaximumTemperature(unsigned int* index)
{
    float pixelsTemperature[PIXEL_NUM] = {0};
    float maxTemp = 0;
    tsa.read_pixel_temperature(pixelsTemperature);     // Read temperature of all the 64 pixels

    for(int i = 0; i < 64; i++)
    {
        if(pixelsTemperature[i] > maxTemp)
        {
            maxTemp = pixelsTemperature[i];
            *index = i;
        }
    }
    return maxTemp;    
}

bool getAcceleration(double* xyz)
{
    adxl.getAcceleration(xyz);
    return true;    
}

void setup() 
{
    Serial.begin(2000000);                                      // Initialize Serial Port 
    lcd.begin(16, 2);                                           // Initialize LCD (LCD Start!)
    lcd.setRGB(0, 255, 0);                                      // Green LCD Back Light
    analogReadResolution(10);                                   // Initialize Analog Resolution to 10 bits.
    ADC->CTRLB.bit.PRESCALER = ADC_CTRLB_PRESCALER_DIV64_Val;   // Set the prescaler for ADC clock (e.g., DIV64 for balanced speed/accuracy)
    if (tsa.init()) {                                           // Initialize AMG8833 TSA
        Serial.println("TSA Init Failed!!!");                   // If TSA initialize failed, Print Failed Message to Serial Port
        lcd.setRGB(255, 0, 0);                                  // Set Red LCD Back Light
        lcd.print("TSA FAILED!");                               // Print Failed Message to LCD
        while (1);                                              // Stop Here!
    }
    Serial.println("AMG8833 Init OK!!!");                       // If TSA initialize successfull, Print, Success Message to Serial Port
    adxlInitialize();                                           // Initialize Accelerometer  
}

void loop() 
{
    unsigned int soundLevel = 0;                                        // Stores peak-to-peak sound level (Raw value)
    unsigned int maxTempIndex = 0;                                      // Index of Maximum Temperature on TSA 
    float maxTemp;                                                      // Maximum Temperature
    double xyz[3];                                                      // X, Y, Z acceleration values (g)
    double xyzMag;                                                      // Acceleration magnitude |XYZ| 
    soundLevel = getSoundLevel(SOUNDSENSOR);                            // Read Sound Pressure from Sound Sensor
    maxTemp    = getMaximumTemperature(&maxTempIndex);                  // Read Maximum non-contact Temperature and its Index from TSA    
    getAcceleration(xyz);                                               // Read acceleration values 
    xyzMag = (xyz[0] * xyz[0]) + (xyz[1] * xyz[1]) + (xyz[2] * xyz[2]); // Calculate |XYZ| acceleration magnitude 
    xyzMag = sqrt(xyzMag);                                              // Calculate |XYZ| acceleration magnitude 

    // Implements simple thresholds checks for monitoring of machine health (Todo: Need Improvements!)
    if(soundLevel > SOUND_THRESHOLD || maxTemp > TEMP_THRESHOLD || xyzMag > VIBR_THRESHOLD)
    {
        lcd.clear();
        lcd.setRGB(255, 0, 0);                                            // Set Red LCD Back Light
        lcd.print("[W] Mac Check!");                                      // Print Warning Message to LCD when threshold exceeded
    }
    else
    {
        lcd.clear();
        lcd.setRGB(0, 255, 0);                                            // Set Green LCD Back Light
        lcd.print("[I] Mac OK!");                                         // Print Information Message to LCD when thresholds are within range  
    }    

    // Todo: Send Sensor values to Cloud Dashboard.

    // Print all sensor values to Serial Port. 
    Serial.print(soundLevel);
    Serial.print(" ");
    Serial.print(maxTemp);
    Serial.print(" ");
//  Serial.print(maxTempIndex);
//  Serial.print(" ");
    Serial.println(xyzMag);            
}