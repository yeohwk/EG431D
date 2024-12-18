#include <math.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Seeed_AMG8833_driver.h"

#define TEMP_THRESHOLD    40    // Max Temperature Threshold (degree C)
#define VIBR_THRESHOLD    1.9   // Vibration Threshold (g)

rgb_lcd lcd;                    // Initialize LCD instance
AMG8833 tsa(0x69);              // Initialize AMG8833 instance of the  AMG8833 TSA library (use address 0x68 or 0x69)

float getMaximumTargetTemperature(unsigned int* index)
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
    // At this point, maxTemp is the apparent temperature measured by the TSA.
    // Need to apply the provided equation to obtain the Target Temperature. 

    

    return maxTemp;    
}

void setup() 
{
    Serial.begin(2000000);                                      // Initialize Serial Port 
    lcd.begin(16, 2);                                           // Initialize LCD (LCD Start!)
    lcd.setRGB(0, 255, 0);                                      // Green LCD Back Light
    if (tsa.init()) {                                           // Initialize AMG8833 TSA
        Serial.println("TSA Init Failed!!!");                   // If TSA initialize failed, Print Failed Message to Serial Port
        lcd.setRGB(255, 0, 0);                                  // Set Red LCD Back Light
        lcd.print("TSA FAILED!");                               // Print Failed Message to LCD
        while (1);                                              // Stop Here!
    }
    Serial.println("AMG8833 Init OK!!!");                       // If TSA initialize successfull, Print, Success Message to Serial Port
}

void loop() 
{
    static unsigned int maxTempIndex = 0;                                  // Index of Maximum Temperature on TSA 
    static float maxTemp = 0.0;                                            // Maximum Temperature
    maxTemp = getMaximumTargetTemperature(&maxTempIndex);                  // Read Maximum non-contact Temperature and its Index from TSA 





    // Implements simple thresholds checks for monitoring of machine health (Todo: Need Improvements!)
    if(soundLevel > SOUND_THRESHOLD || maxTemp > TEMP_THRESHOLD || xyzMag > VIBR_THRESHOLD)
    {
        lcd.clear();
        lcd.setRGB(255, 0, 0);                                           // Set Red LCD Back Light
        lcd.print("[W] Mac Check!");                                     // Print Warning Message to LCD when threshold exceeded
    }
    else
    {
        lcd.clear();
        lcd.setRGB(0, 255, 0);                                           // Set Green LCD Back Light
        lcd.print("[I] Mac OK!");                                        // Print Information Message to LCD when thresholds are within range  
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