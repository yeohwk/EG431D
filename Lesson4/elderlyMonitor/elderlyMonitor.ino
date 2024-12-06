#include <math.h>
#include <Wire.h>
#include "rgb_lcd.h"                    // RGB LCD Library
#include "Seeed_AMG8833_driver.h"       // AMH8833 TSA Library 
#include "SCD30.h"                      // CO2, Temperature and Humidity Sensor Library

#define PIRSENSOR             0         // Arduino D0 digital pin connects to PIR Sensor
#define SOUNDSENSOR           A2        // Arduino A2 analog pin  connects to Sound Sensor 
#define SOUNDSAMPLEWINDOW     50        // 50ms sound sampling window
#define SOUND_THRESHOLD       20        // Sound Level Threshold (raw)
#define TSATEMP_THRESHOLD     28        // Max TSA Temperature Threshold (degree C) to detect human
#define INACTIVITY_THRESHOLD  10000     // 3 Minutes Inactivity Threshold (180 seconds x 1000ms) 

#define CO2_THRESHOLD         900
#define ATEMP_THRESHOLD       27
#define HUMIDITY_THRESHOLD    50

rgb_lcd lcd;                            // Initialize LCD instance
AMG8833 tsa;                            // Initialize AMG8833 instance of the  AMG8833 TSA library
float co2th[3] = {0};                   // CO2, Ambient Temp and Humidity Values.
unsigned long loopTimeBegin   = 0;      // Time variable - for use in loop ()
unsigned long loopTimeElapsed = 0;      // Time variable - for use in loop ()

unsigned long motionActCount  = 0;      // Motion detected Counter. Increment by 1 when motion activity is detected
bool motionActWarning = false;          // Motion In-Activity Warning. Set to True when the in-activity time exceeds threshold
        
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
        if (soundSample < 1024)                         // toss out spurious readings
        {
            if (soundSample > soundSignalMax)
            {
                soundSignalMax = soundSample;           // save just the max levels
            }
            else if (soundSample < soundSignalMin)
            {
                soundSignalMin = soundSample;           // save just the min levels
            }
        }
    }
    soundLevel = soundSignalMax - soundSignalMin;       // max - min = peak-peak amplitude
    return soundLevel;       
}

float getMaximumTemperature(unsigned int* index)
{
    float pixelsTemperature[PIXEL_NUM] = {0};
    float maxTemp = 0;
    tsa.read_pixel_temperature(pixelsTemperature);              // Read temperature of all the 64 pixels

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

bool getCO2TempHumidity(float* result)
{
    if (scd30.isAvailable())                                      // Check for Sensor availability
    {
        scd30.getCarbonDioxideConcentration(result);              // Get CO2, Temp and Humidity data from sensor
        return true;
    }
    else
      return false;  
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
    scd30.initialize();                                         // Initialize Grove – CO2, Temperature, and Humidity Sensor 
    pinMode(PIRSENSOR, INPUT);
}

void loop() 
{
    unsigned int soundLevel = 0;                                  // Stores peak-to-peak sound level (Raw value)
    unsigned int maxTempIndex = 0;                                // Index of Maximum Temperature on TSA 
    float maxTemp;                                                // Maximum Temperature
    bool  motion;                                                 // Detected motion from PIR sensor 

    float CO2;                                                     
    float ATemperature;                                             
    float Humidity;                                               

    loopTimeBegin = millis();                                     // Read start time 
    getCO2TempHumidity(co2th);                                    // Read CO2, Ambient Temperature and Humidity values
    soundLevel = getSoundLevel(SOUNDSENSOR);                      // Read Sound Pressure from Sound Sensor
    maxTemp = getMaximumTemperature(&maxTempIndex);               // Read Maximum non-contact Temperature and its Index from TSA    
    motion  = digitalRead(PIRSENSOR);

    CO2 = co2th[0];                                               // CO2 concentration (ppm)
    ATemperature = co2th[1];                                      // Ambient Temperature (degree C) 
    Humidity = co2th[2];                                          // Humidity Level (%)

    // If measured inactivity time is greater or equals to the time threshold or motion is detected
    if ((loopTimeElapsed >= INACTIVITY_THRESHOLD) || (motion == true))
    {
        loopTimeElapsed = 0;                                      // Reset inactivity time
        if (motion == false && maxTemp > TSATEMP_THRESHOLD)       // No Motion activity and TSA indicates presence of human,
            motionActWarning = true;                              // Set Motion In-Activity Warning
        else
        {
            motionActWarning = false;                             // Clear Motion In-Activity Warning
            if(motion == true)
                motionActCount = motionActCount + 1;              // Not use for now (Can think of something useful?)
        }
    }
            
    // Implements simple thresholds checks for loud sound and elderly motion activity monitoring (Todo: Need Improvements!)
    if(soundLevel > SOUND_THRESHOLD)
    {
        lcd.clear();
        lcd.setRGB(255, 0, 0);                                            
        lcd.print("[W] LOUD SND!");
    }
    else if ( motionActWarning == true)
    {
        lcd.clear();
        lcd.setRGB(255, 0, 0);                                            
        lcd.print("[W] NO MOTION!"); 
    }
    else
    {
        lcd.clear();
        lcd.setRGB(0, 255, 0);
        lcd.print("[I] NORMAL") ;  
    }

    // Implements simple thresholds checks for environmental variables (Todo: Need Improvements!)
    if (CO2 >= CO2_THRESHOLD)
    {
        Serial.println("Warning: High CO2!");       
    }

    if (ATemperature >= ATEMP_THRESHOLD)
    {  
        Serial.println("Warning: High Ambient Temperature!");       
    }

    if (Humidity >= HUMIDITY_THRESHOLD)
    {  
        Serial.println("Warning: High Humidity");       
    }

    // Todo: Send Sensor values to Cloud Dashboard.

    // Print all sensor values to Serial Port. 
    Serial.print(soundLevel);
    Serial.print(" ");
    Serial.print(maxTemp);
    Serial.print(" ");
    Serial.print(motionActWarning);
    Serial.print(" ");
    Serial.print(CO2);
    Serial.print(" ");
    Serial.print(ATemperature);    
    Serial.print(" ");
    Serial.println(Humidity);    

    loopTimeElapsed = loopTimeElapsed + (millis() - loopTimeBegin);
}