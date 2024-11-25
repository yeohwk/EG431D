#include <Wire.h>
#include <Digital_Light_TSL2561.h>

void setup() {
    Wire.begin();       // Initialize I2C Communication
    Serial.begin(9600); // Initialize Serial Port
    TSL2561.init();     // Initialize Light Sensor
}

void loop() {
    Serial.print("The Light value (Lux) is: ");
    Serial.println(TSL2561.readVisibleLux());
    delay(1000);
}


