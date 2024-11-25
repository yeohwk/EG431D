#include <math.h>
#define TEMP_SENSOR  A1       // Grove - Temperature Sensor connects to Analog Port A1
#define LED1         5        // LED1 connects to Digital Port D5 (LED is Active LOW)
#define LED2         6        // LED2 connects to Digital Port D6 (LED is Active LOW)
#define TEMP_TH      27       // Temperature Threshold
const int B  = 4275;          // B value of the thermistor (from Datasheet)
const int R0 = 100000;        // R0 = 100k
bool _led1 = 1;               // LED1 State (1 = LED OFF, 0 = LED ON)
bool _led2 = 1;               // LED2 State (1 = LED OFF, 0 = LED ON)
 
void setup()
{
    Serial.begin(9600);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    analogReadResolution(12); // Initialize Analog Resolution to 12 bits.
}

void loop()
{
    int a = analogRead(TEMP_SENSOR);
    float R = 4095.0/a - 1.0;
    R = R0 * R;

    // converts R to temperature using simplified Steinhart–Hart equation
    float temperature = 1.0/(log(R/R0)/B + 1/298.15) - 273.15; 
    Serial.print("temperature = ");
    Serial.println(temperature);

    if (temperature > TEMP_TH)
    {
      _led1 = !(_led1);         // Blink LED1
      _led2 = 0;                // Turns ON LED2
    }
    else
      _led1 = 1;                // Turns OFF LED1

    digitalWrite(LED1, _led1);  // Write _led1 State to Port D5  
    digitalWrite(LED2, _led2);  // Write _led2 State to Port D6
    delay(500);
}
