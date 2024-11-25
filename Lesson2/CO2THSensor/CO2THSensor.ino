#include "SCD30.h"
#define SERIAL Serial

void setup() {
    Wire.begin();                     // Initialize I2C Communication
    SERIAL.begin(115200);             // Initialize Serial Port
    SERIAL.println("SCD30 Raw Data");
    scd30.initialize();               // Initialize Grove – CO2, Temperature, and Humidity Sensor 
}

void loop() {
    float result[3] = {0};
    if (scd30.isAvailable()) {
        scd30.getCarbonDioxideConcentration(result);
        SERIAL.print("Carbon Dioxide Concentration is: ");
        SERIAL.print(result[0]);
        SERIAL.println(" ppm");
        SERIAL.println(" ");
        SERIAL.print("Temperature = ");
        SERIAL.print(result[1]);
        SERIAL.println(" ℃");
        SERIAL.println(" ");
        SERIAL.print("Humidity = ");
        SERIAL.print(result[2]);
        SERIAL.println(" %");
        SERIAL.println(" ");
        SERIAL.println(" ");
        SERIAL.println(" ");
    }
    delay(2000);
}

