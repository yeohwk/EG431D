#include <Wire.h>
#include <Digital_Light_TSL2561.h>

void setup() {
    Wire.begin();
    Serial.begin(9600);
    TSL2561.init();
}

void loop() {
    Serial.print("The Infrared intensity value (Raw) is: ");
    // read Infrared channel raw intensity value only, not converted to Lux
    Serial.println(TSL2561.readIRLuminosity()); 

    Serial.print("The Full Spectrum intensity value (Raw) is: ");
    // read Full Spectrum channel raw intensity value only, not converted to Lux
    Serial.println(TSL2561.readFSpecLuminosity());  
    delay(1000);
}
