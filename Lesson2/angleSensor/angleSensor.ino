#define ROTARY_ANGLE_SENSOR A0  // Angle Sensor connects to Arduino Analog A0 Port
#define ADC_REF       3.3       // Reference voltage of ADC is 3.3V.
#define FULL_VOLTAGE  3.3       // Full Scale Voltage of the Angle Sensor is at 3.3V (after attenuation) 
#define FULL_ANGLE    300       // Full value of the rotary angle is 300 degrees

void setup()
{
    Serial.begin(9600);                   // Debug Serial Port
    analogReadResolution(12);             // Initialize Analog Resolution to 12 bits.
}

void loop()
{   
    float voltage;
    int sensor_value = analogRead(ROTARY_ANGLE_SENSOR);   // Read value from Sensor
    voltage = (float)sensor_value * ADC_REF/4095;         // Converts raw ADC value to voltage 
    float degrees = (voltage*FULL_ANGLE)/FULL_VOLTAGE;    // Change voltage to Angle
    Serial.println("The angle between the mark and the starting position:");
    Serial.println(degrees);
    delay(100);
}