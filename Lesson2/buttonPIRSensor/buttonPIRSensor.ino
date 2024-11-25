#define BUTTON 0              // Grove - Button connects to Arduino Digital D0 Port
#define PIR    1              // PIR Motion Sensor connects to Arduino Digital D1 Port
#define BUZZER 2              // Grove - Buzzer connects to Arduino Digital D2 Port

void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON, INPUT);   // Set BUTTON (D0) as digital input
    pinMode(PIR, INPUT);      // Set PIR (D1) as digital input
    pinMode(BUZZER, OUTPUT);  // Set BUZZER (D2) as digital output
}

void loop()
{
    bool _button = digitalRead(BUTTON); // Read BUTTON (D0)
    bool _pir    = digitalRead(PIR);    // Read PIR (D1)
    if(_button == true && _pir == true) // Check if both BUTTON and PIR are asserted
    {
      digitalWrite(BUZZER, HIGH);       // If asserted, BUZZER (D2) is set to HIGH
    }
    else
    {
      digitalWrite(BUZZER, LOW);        // BUZZER (D2) is set to LOW if not asserted
    }
    Serial.print("Button = ");
    Serial.print(_button);
    Serial.print("\t");  
    Serial.print("PIR = ");
    Serial.println(_pir);
    delay(100);
}

