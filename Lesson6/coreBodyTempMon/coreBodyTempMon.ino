#include <math.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Adafruit_AMG88xx.h"   // Replace with the new library from Adafruit
#include <MovingAverageFilter.h>

#define BUZZER_MODULE     2
#define NUM_POINTS        32    // Filter Size (number of measurement points)

#define STATE_NORMAL      1     // State ID for STATE NORMAL
#define STATE_FEVER       2     // State ID for STATE FEVER
#define STATE_MFEVER      3     // State ID for STATE MODERATE FEVER   
#define STATE_HFEVER      4     // State ID for STATE HIGH FEVER       

rgb_lcd lcd;                                          // Initialize LCD instance
Adafruit_AMG88xx tsa;                                 // Create an AMG8833 instance
MovingAverageFilter movingAverageFilter(NUM_POINTS);  // Initialize Moving Average Filter

float getMaximumTargetTemperature(unsigned int* index)
{
    float pixelsTemperature[AMG88xx_PIXEL_ARRAY_SIZE] = {0};
    float apparentTemp = 0;
    double targetTemp  = 0;
    double ambientTemp = 0;

    // Read temperature of all the 64 pixels (temperature sensors)
    tsa.readPixels(pixelsTemperature);                
    // Get the maximum apparent temperature value from the 64 pixels
    for(int i = 0; i < 64; i++)
    {
        if(pixelsTemperature[i] > apparentTemp)
        {
            apparentTemp = pixelsTemperature[i];
            *index = i;
        }
    }
    // At this point, maxTemp is the apparent temperature measured by the TSA.
    // Need to pre-process with a moving average filter to remove noise.
    // Need to apply the provided equation to obtain the Target Temperature.
    apparentTemp = movingAverageFilter.process(apparentTemp);         // Pre-process with moving average filter
    apparentTemp = apparentTemp + (float) 273.15;                     // Converts apparent temperature from deg C to Kelvin 
    ambientTemp  = (double) tsa.readThermistor() + (double) 273.15;   // Read Thermistor temperature (ambient temperature) and converts to Kelvin
    ambientTemp  = pow(ambientTemp, (double) 4.0);                    // Implementation of formula to calculate target temperature from measured 
    ambientTemp  = (double) (0.02) * ambientTemp;                     // apparent temperature, ambient temperature and emissivity values.
    targetTemp   = pow((double) apparentTemp, (double) 4.0);          // 
    targetTemp   = targetTemp - ambientTemp;                          //
    targetTemp   = targetTemp / (double) 0.98;                        //  
    targetTemp   = sqrt(targetTemp);                                  //
    targetTemp   = sqrt(targetTemp);                                  //
    return (float) (targetTemp) - (float) 273.15;                     // Calculated target temperature converts to deg C
}

void setup() 
{
    bool status;
    Wire.begin();
    Serial.begin(2000000);                                      // Initialize Serial Port
    pinMode(BUZZER_MODULE, OUTPUT);                             // Set BUZZER_MODULE (D2) as digital output 
    lcd.begin(16, 2);                                           // Initialize LCD (LCD Start!)
    lcd.setRGB(0, 255, 0);                                      // Green LCD Back Light
    status = tsa.begin(0x69, &Wire);                            // Initialize TSA (use address 0x68 or 0x69)
    if (status == 0) {                                          // Check TSA initialization status.
        Serial.println("TSA Init Failed!!!");                   // If TSA initialize failed, Print Failed Message to Serial Port
        lcd.setRGB(255, 0, 0);                                  // Set Red LCD Back Light
        lcd.print("TSA FAILED!");                               // Print Failed Message to LCD
        while (1);                                              // Stop Here!
    }
    Serial.println("AMG8833 Init OK!!!");                       // If TSA initialize successfull, Print, Success Message to Serial Port
}

void loop() 
{
    // STATE variables
    static unsigned char STATE = STATE_NORMAL;                  // Store State ID
    static unsigned int TB_I = 0;                               // CBT Index on TSA 
    static float TB = 0.0;                                      // CBT (Maximum non-contact temperature)
    static unsigned int BUZ_CT = 0;
    static bool BUZZER = false;

    // Read sensors (Input)
    // Read maximum non-contact temperature (CBT) and its Index from TSA 
    TB = getMaximumTargetTemperature(&TB_I);

    // Run Finite State Machine (FSM) logic
    switch (STATE)
    {
        case STATE_NORMAL:
          if (TB < 37.5)
          {}
          else if (TB>=37.5 && TB < 38.3)
          {
              STATE = STATE_FEVER;
              BUZ_CT = 4;
          }
          else if (TB>=38.3 && TB < 39)
          {
              STATE = STATE_MFEVER;
          }
          else if (TB >= 39)
          {
              STATE = STATE_HFEVER;
              BUZZER = true;
          }
          else
          {}
        break;
        case STATE_FEVER:
          if (TB>=37.5 && TB < 38.3 && BUZ_CT == 0)
          {
              BUZ_CT = 4;
              BUZZER = true;
          }
          else if (TB>=37.5 && TB < 38.3 && BUZ_CT > 0)
          {
              BUZ_CT = BUZ_CT - 1;
              BUZZER = false;              
          }
          else if (TB < 37.5 || TB >= 38.3)
          {
              STATE = STATE_NORMAL;
              BUZZER = false;
          }
          else
          {}
        break;
        case STATE_MFEVER:
          if (TB >= 38.3 && TB < 39)
          {
              BUZZER = !BUZZER;
          }
          else if (TB < 38.3 || TB >= 39)
          {
              STATE = STATE_NORMAL;
              BUZZER = false;
          }
          else
          {}
        break;
        case STATE_HFEVER:
          if (TB >= 39)
          {}
          else if (TB < 39)
          {
              STATE = STATE_NORMAL;
              BUZZER = false;
          }
          else
          {}
        break;
        default:
        break;  
    } 

    // Update Outputs
    // Print results to LCD and Serial Monitor
    // Todo: Send results to cloud dashboard for remote monitoring purpose
    lcd.clear();
    if (STATE == STATE_NORMAL)
    {
      lcd.setRGB(0, 255, 0);                            // Green LCD Back Light 
      lcd.setCursor(0, 0); lcd.print("[STAT]: "); lcd.print("NORMAL");
      Serial.print("STATE_NORMAL: ");    
    }
    else if (STATE == STATE_FEVER)
    {
      lcd.setRGB(0, 0, 255);                            // Blue LCD Back Light 
      lcd.setCursor(0, 0); lcd.print("[STAT]: "); lcd.print("FEVER!");
      Serial.print("STATE_FEVER: ");     
    }
    else if (STATE == STATE_MFEVER)
    {
      lcd.setRGB(255, 255, 0);                          // Yellow LCD Back Light 
      lcd.setCursor(0, 0); lcd.print("[STAT]: "); lcd.print("MFEVER!");
      Serial.print("STATE_MFEVER: ");     
    }
    else
    {
      lcd.setRGB(255, 0, 0);                            // Red LCD Back Light 
      lcd.setCursor(0, 0); lcd.print("[STAT]: "); lcd.print("HFEVER!");
      Serial.print("STATE_HFEVER: ");       
    }
    lcd.setCursor(0, 1); lcd.print("[CBT ]: "); lcd.print(TB);

    // Print results to Serial Monitor
    Serial.print("CBT: ");
    Serial.print(TB);
    Serial.print(" deg C, CBTi: ");
    Serial.println(TB_I);

    // Update BUZZER_MODULE (output module) at every cycle
    digitalWrite(BUZZER_MODULE, BUZZER);
    // 100ms FSM time reference (Runs at every 100ms)
    delay(100);
}