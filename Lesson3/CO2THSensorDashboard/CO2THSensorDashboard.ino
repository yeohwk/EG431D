#include <AllThingsTalk_WiFi.h> // AllThingsTalk Library
#include <WiFiNINA.h>           // WiFi Library (Arduino MKR WiFi 1010).
#include <utility/wifi_drv.h>   // Exposes MKR1010's underlying functions to control the RGB LED
#include "ArduinoJson.h"        // Arduino JSON Library
#include "Scheduler.h"          // Arduino Scheduler Library
#include "SCD30.h"              // CO2, Temperature and Humidity Sensor Library
#define SERIAL Serial

auto wifiCreds   = WifiCredentials("WiFiSSID", "WiFiPassword");   // Your WiFi Network Name and Password
auto deviceCreds = DeviceConfig("DeviceID", "DeviceToken");       // Go to AllThingsTalk Maker > Devices > Your Device > Settings > Authentication to get your Device ID and Token
auto device      = Device(wifiCreds, deviceCreds);                // Create "device" object
char* CO2_SENSOR  = "CO2_Sensor";                                 // (ATT Asset) CO2_Sensor
char* TEMP_SENSOR = "Temperature_Sensor";                         // (ATT Asset) Temperature_Sensor
char* HUMI_SENSOR = "Humidity_Sensor";                            // (ATT Asset) Humidity_Sensor

void setup() {
    Wire.begin();               // Initialize I2C Communication
    SERIAL.begin(115200);       // Initialize Serial Port
    scd30.initialize();         // Initialize Grove – CO2, Temperature, and Humidity Sensor 
    device.debugPort(Serial);   // Set AllThingsTalk library to output its debug to "Serial"
    device.init();              // Initialize WiFi and AllThingsTalk
}

void loop() {
    float result[3] = {0};
    if (scd30.isAvailable()) {                        // Check for Sensor availability
        scd30.getCarbonDioxideConcentration(result);  // Get CO2, Temp and Humidity data from sensor
        SERIAL.print(result[0]);                      // CO2 Concentration on PPM
        SERIAL.print(",");                            // Delimiter (comma)
        SERIAL.print(result[1]);                      // Ambient Temperature in Degree C
        SERIAL.print(",");                            // Delimiter (comma)
        SERIAL.println(result[2]);                    // Humidity in %
        device.send(CO2_SENSOR,  (int) result[0]);    // Send CO2 data to AllThingsTalk
        delay(1000);
        device.send(TEMP_SENSOR, (int) result[1]);    // Send Temperature data to AllThingsTalk
        delay(1000);
        device.send(HUMI_SENSOR, (int) result[2]);    // Send Humidity data to AllThingsTalk
        delay(1000);

    }
    delay(2000);
    device.loop();                                    // Keep AllThingsTalk connection alive
}
