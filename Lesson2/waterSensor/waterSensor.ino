#define WATER_SENSOR A3         // Grove - Water Sensor connects to Arduino Analog A3 Port

void setup()
{
    Serial.begin(115200);
    analogReadResolution(12);    // Initialize Analog Resolution to 12 bits.
}

void loop()
{
    long sum = 0;
    
    // Read 16 measured conductivity samples
    for(int i=0; i < 16; i++)
    {
        sum += analogRead(WATER_SENSOR);
    }

    // Calculate an average sound from the 16 conductivity samples.
    sum = sum / 16; 
    Serial.println(sum);          
    delay(1);
}
