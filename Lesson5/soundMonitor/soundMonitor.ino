#define SOUND_SENSOR A2           // Grove - Sound Sensor connects to Arduino Analog A2 Port

void setup()
{
    Serial.begin(115200);
    analogReadResolution(10);     // Initialize Analog Resolution to 10 bits (for speed)
}

void loop()
{
    long sound = 0;
    sound = analogRead(SOUND_SENSOR);
    Serial.println(sound);          
}
