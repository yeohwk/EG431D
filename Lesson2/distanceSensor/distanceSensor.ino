#include "Ultrasonic.h"
#define ULTRASONIC_RANGER 3               // Grove - Ultrasonic Ranger connects to Arduino D3 Port

Ultrasonic ultrasonic(ULTRASONIC_RANGER); // Initialize Grove - Ultrasonic Ranger
void setup()
{
    Serial.begin(115200);                 // Initialize Serial Port
}

void loop()
{
  long RangeInInches;
  long RangeInCentimeters;
  Serial.println("The distance to a target obstacle: ");
  RangeInInches = ultrasonic.MeasureInInches();
  Serial.print(RangeInInches);      // 0~157 inches
  Serial.println(" inch");
  delay(250);

  // keep an interval between two measurements 
  RangeInCentimeters = ultrasonic.MeasureInCentimeters(); 
  Serial.print(RangeInCentimeters); // 0~400cm
  Serial.println(" cm");
  delay(250);
}

