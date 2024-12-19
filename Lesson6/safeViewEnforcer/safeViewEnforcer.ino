#include <Wire.h>
#include "rgb_lcd.h"
#include "Ultrasonic.h"
#include "MedianFilterLib.h"
#define BUZZER_MODULE     2                           // Grove - Buzzer connects to Arduino Digital D2 Port
#define ULTRASONIC_RANGER 3                           // Grove - Ultrasonic Ranger connects to Arduino D3 Port
#define NUM_POINTS        5                           // Filter Size (number of measurement points)

#define STATE_SAFE        1                           // State ID for STATE SAFE
#define STATE_REMINDER    2                           // State ID for STATE REMINDER
#define STATE_WARNING     3                           // State ID for STATE WARNING   
#define STATE_TVOFF       4                           // State ID for STATE TVOFF       

rgb_lcd lcd;                                          // Initialize LCD
Ultrasonic ultrasonic(ULTRASONIC_RANGER);             // Initialize Grove - Ultrasonic Ranger
MedianFilter<float> medianFilter(NUM_POINTS);         // Initialize Median Filter.

void setup()
{
    Serial.begin(115200);                             // Initialize Serial Port
    lcd.begin(16, 2);                                 // Init LCD
    lcd.setRGB(0, 255, 0);                            // Green LCD Back Light    
    pinMode(BUZZER_MODULE, OUTPUT);                   // Set BUZZER_MODULE (D2) as digital output
}

void loop()
{
    // STATE variables
    static unsigned char STATE   = STATE_SAFE;        // Store State ID
    static unsigned int  TIMER   = 0;                 // Power-Off Timer (e.g. TIMER = 100 indicates TV to be power off after 100ms x 100 = 10 seconds)
    static unsigned int  BUZ_CTR = 0;                 // Variable for generating beeping sound (e.g. BUZ_CTR = 5 indicates buzzer to beep at every 500ms)
    static float DISTANCE = 0;                        // Store distance information from sensor
    static bool TVON = true;                          // Output variable to indicate TV ON/OFF
    static bool BUZZER = false;                       // Output variable to indicate Buzzer ON/OFF

    // Read sensors (Input)
    // Read distance value from sensor and feed into a median filter
    DISTANCE = medianFilter.AddValue(ultrasonic.MeasureInCentimeters());

    // Run Finite State Machine (FSM) logic
    switch (STATE)
    {
        case STATE_SAFE:
          if (DISTANCE >= 220)
          {}
          else if (DISTANCE >= 150 && DISTANCE < 220)
          {
              STATE = STATE_REMINDER;
              BUZ_CTR = 5;
          }
          else if (DISTANCE < 150)
          {
              STATE = STATE_WARNING;
              TIMER = 100;
          }
          else
          {}
        break;
        case STATE_REMINDER:
          if (DISTANCE >= 150 && DISTANCE < 220 && BUZ_CTR > 0)
          {
              BUZ_CTR = BUZ_CTR - 1;
              BUZZER = false;
          }
          else if (DISTANCE >= 150 && DISTANCE < 220 && BUZ_CTR == 0)
          {
              BUZ_CTR = 5;
              BUZZER = true;
          }
          else if (DISTANCE >= 220)
          {
              STATE = STATE_SAFE;
              BUZZER = false;
          }
          else if (DISTANCE < 150)
          {
              STATE = STATE_WARNING;
              TIMER = 100;
          }
          else
          {}        
        break;
        case STATE_WARNING:
          if (DISTANCE < 220 && TIMER > 0)
          {
              TIMER = TIMER - 1;
              BUZZER = !BUZZER;
          }
          else if (DISTANCE >= 220)
          {
              STATE = STATE_SAFE;
              BUZZER = false;
          }
          else if (TIMER ==0)
          {
              STATE = STATE_TVOFF;
              TVON = false;
              BUZZER = false;
          }
          else
          {}
        break;
        case STATE_TVOFF:
          if (DISTANCE < 220)
          {}
          else if (DISTANCE >= 220)
          {
              STATE = STATE_SAFE;
              TVON = true;
          }
          else
          {}
        break;
        default:
        break;
    }

    // Update Outputs
    // Prints result to LCD and Serial Monitor
    lcd.clear();
    if (STATE == STATE_SAFE)
    {
      lcd.setRGB(0, 255, 0);                            // Green LCD Back Light 
      lcd.setCursor(0, 0); lcd.print("[STAT]: "); lcd.print("SAFE");
      Serial.print("STATE_SAFE: ");    
    }
    else if (STATE == STATE_REMINDER)
    {
      lcd.setRGB(255, 255, 0);                          // Yellow LCD Back Light 
      lcd.setCursor(0, 0); lcd.print("[STAT]: "); lcd.print("REMINDER");
      Serial.print("STATE_REMINDER: ");     
    }
    else if (STATE == STATE_WARNING)
    {
      lcd.setRGB(255, 0, 0);                            // Red LCD Back Light 
      lcd.setCursor(0, 0); lcd.print("[STAT]: "); lcd.print("WARNING!");
      Serial.print("STATE_WARNING: ");     
    }
    else
    {
      lcd.setRGB(255, 0, 0);                            // Red LCD Back Light 
      lcd.setCursor(0, 0); lcd.print("[STAT]: "); lcd.print("TVOFF!");
      Serial.print("STATE_TVOFF: ");       
    }
    lcd.setCursor(0, 1); lcd.print("[DIST]: "); lcd.print(DISTANCE);

    // Prints result to Serial Monitor
    Serial.print("Distance to User: ");
    Serial.print(DISTANCE);
    Serial.println(" cm");

    // Update BUZZER_MODULE (output module) at every cycle
    digitalWrite(BUZZER_MODULE, BUZZER);
    // Do something based on TVON output variable (Optional)
    if (TVON == false)
    {
      // Do Something (User Defined)!
    }

    // 100ms FSM time reference (Runs at every 100ms)
    delay(100);
}
