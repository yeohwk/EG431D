#include <Wire.h>
#include "rgb_lcd.h"
#include "Ultrasonic.h"
#define ULTRASONIC_RANGER 3               // Grove - Ultrasonic Ranger connects to Arduino D3 Port
#define NUM_POINTS        32              // Number of measurement points

rgb_lcd lcd;                              // Initialize LCD
Ultrasonic ultrasonic(ULTRASONIC_RANGER); // Initialize Grove - Ultrasonic Ranger
void setup()
{
    Serial.begin(115200);                 // Initialize Serial Port
    lcd.begin(16, 2);                     // Init LCD
    lcd.setRGB(0, 255, 0);                // Green LCD Back Light    
}

void loop()
{
  float RangeInInches = 0;
  float RangeInCentimeters = 0;
  Serial.println("The distance to a target obstacle: ");
  
  // Simple averaging of 32 distance measurements.
  // Sampling time: 1 measurement at every 10ms.  
  for(int i = 0; i < NUM_POINTS; i++)
  { 
      // Read distance value from sensor and sum
      RangeInInches = RangeInInches + (float) ultrasonic.MeasureInInches();
      delay(10);
  }
  // Calculates average distance
  RangeInInches = RangeInInches / (float) NUM_POINTS;
  // Converts from Inch to CM
  RangeInCentimeters = RangeInInches * (float) 2.54;        

  // Prints result to Serial Monitor
  Serial.print(RangeInInches);      // 0~157 inches
  Serial.println(" inch");
  Serial.print(RangeInCentimeters); // 0~400cm
  Serial.println(" cm");

  // Prints result to LCD
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("[in]: "); lcd.print(RangeInInches);
  lcd.setCursor(0, 1); lcd.print("[cm]: "); lcd.print(RangeInCentimeters);
  delay(250);
}

