#include <math.h>                   // Math Library
#include <Wire.h>                   // I2C Library
#include <WiFiNINA.h>               // WiFi Library
#include <Digital_Light_TSL2561.h>  // Digital Light Sensor Library
#include "ThingSpeak.h"             // Always include thingspeak header file after other header files
#define TEMP_SENSOR  A1             // Temperature Sensor connects to Analog Port A1
#define SOUND_SENSOR A2             // Sound Sensor connects to Arduino Analog A2 Port
const int B  = 4275;                // B value of the thermistor (from Datasheet)
const int R0 = 100000;              // R0 = 100k
int Temperature = 0;                // Temperature Sensor
int Sound       = 0;                // Sound Sensor
int Light       = 0;                // Light Intensity

unsigned long myChannelNumber = 0000000;            // Replace 0000000 with your channel number
const char * myWriteAPIKey    = "WRITE_APIKEY";     // Replace WRITE_APIKEY with your channel write API Key
char ssid[]  = "MYSSID";                            // Replace MYSSID with your WiFi network name
char pass[]  = "MYPASSWORD";                        // Replace MYPASSWORD with your WiFi password
WiFiClient  client;                                 // Initialize WiFi

void setup() {
    Wire.begin();                                   // Initialize I2C Communication
    Serial.begin(115200);                           // Initialize serial
    analogReadResolution(12);                       // Initialize Analog Resolution to 12 bits.
    TSL2561.init();                                 // Initialize Light Sensor

    if (WiFi.status() == WL_NO_MODULE)              // Check for WiFi module:
    {
        Serial.println("Communication with WiFi module failed!");
        while (true);                               // WiFi Module is absent, Don't continue!
    }    
    ThingSpeak.begin(client);                       // Initialize ThingSpeak
}
void loop() {
    if(WiFi.status() != WL_CONNECTED)               // Connect or reconnect to WiFi
    {
        Serial.print("Attempting to connect to SSID: ");    
        Serial.println(ssid);

        while(WiFi.status() != WL_CONNECTED){
            WiFi.begin(ssid, pass);                 // Connect to WPA/WPA2 network
            Serial.print(".");
            delay(5000);     
        } 
        Serial.println("\nConnected.");
    }
    int   a = analogRead(TEMP_SENSOR);              // Read Temperature Sensor raw value
    float R = 4095.0/a - 1.0;
    R = R0 * R;
    // converts R to temperature using simplified Steinhart–Hart equation
    float temperature = 1.0/(log(R/R0)/B + 1/298.15) - 273.15;    
    Temperature = (int) temperature;
    Sound = analogRead(SOUND_SENSOR);               // Read Sound Sensor raw value (Sound Level)
    Light = TSL2561.readVisibleLux();               // Read visible light intensity (Lux) 
    ThingSpeak.setField(1, Temperature);            // Update ThingSpeak Field1 (Temperature)
    ThingSpeak.setField(2, Light);                  // Update ThingSpeak Field2 (LightIntensity)
    ThingSpeak.setField(3, Sound);                  // Update ThingSpeak Field3 (SoundLevel)
    ThingSpeak.setStatus("Sensor Values Update");   // Set ThingSpeak Status
    
    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
        Serial.println("Channel update successful.");
    }
    else{
        Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    delay(20000); // Wait 20 seconds to update the channel again
}
