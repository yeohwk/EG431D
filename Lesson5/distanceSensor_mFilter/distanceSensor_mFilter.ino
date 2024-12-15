#include <Wire.h>
#include "rgb_lcd.h"
#include "Ultrasonic.h"
#include "MedianFilterLib.h"
#define ULTRASONIC_RANGER 3                           // Grove - Ultrasonic Ranger connects to Arduino D3 Port
#define NUM_POINTS        5                           // Filter Size (number of measurement points)

rgb_lcd lcd;                                          // Initialize LCD
Ultrasonic ultrasonic(ULTRASONIC_RANGER);             // Initialize Grove - Ultrasonic Ranger
MedianFilter<float> medianFilter(NUM_POINTS);         // Initialize Median Filter.

void setup()
{
    Serial.begin(115200);                             // Initialize Serial Port
    lcd.begin(16, 2);                                 // Init LCD
    lcd.setRGB(0, 255, 0);                            // Green LCD Back Light    
}

void loop()
{
  float RangeInInches = 0;
  float RangeInCentimeters = 0;
  
  // Read distance value from sensor and feed into median filter
  RangeInInches = medianFilter.AddValue(ultrasonic.MeasureInInches());
  // Converts from Inch to CM
  RangeInCentimeters = RangeInInches * (float) 2.54;        

  // Prints result to Serial Monitor
  Serial.println("The distance to a target obstacle: ");
  Serial.print(RangeInInches);      // 0~157 inches
  Serial.println(" inch");
  Serial.print(RangeInCentimeters); // 0~400cm
  Serial.println(" cm");

  // Prints result to LCD
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("[in]: "); lcd.print(RangeInInches);
  lcd.setCursor(0, 1); lcd.print("[cm]: "); lcd.print(RangeInCentimeters);
  delay(10);
}
