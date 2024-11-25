#define SOUND_SENSOR A2         // Grove - Sound Sensor connects to Arduino Analog A2 Port

void setup()
{
    Serial.begin(115200);
    analogReadResolution(12);    // Initialize Analog Resolution to 12 bits.
}

void loop()
{
    long sum = 0;
    
    // Read 32 sound sound samples
    for(int i=0; i < 32; i++)
    {
        sum += analogRead(SOUND_SENSOR);
    }

    // Calculate an average sound from the 32 sound samples.
    sum = sum / 32; 
    Serial.println(sum);          
    delay(1);
}
