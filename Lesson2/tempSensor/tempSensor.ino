#include <math.h>
#define TEMP_SENSOR  A1       // Grove - Temperature Sensor connects to Analog Port A1

const int B  = 4275;          // B value of the thermistor (from Datasheet)
const int R0 = 100000;        // R0 = 100k
   
void setup()
{
    Serial.begin(9600);
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
    delay(100);
}