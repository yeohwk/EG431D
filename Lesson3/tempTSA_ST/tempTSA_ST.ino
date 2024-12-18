#include "Seeed_AMG8833_driver.h"
// Initialize an AMG8833 TSA instance (use address 0x68 or 0x69) 
AMG8833 sensor(0x69);

void setup() {
    Serial.begin(115200);
    
    if (sensor.init()) {
        Serial.println("init failed!!!");
        while (1);
    }
    Serial.println("AMG8833 init OK!!!");
}

void loop() {
    u8 val = 0;
    float pixels_temp[PIXEL_NUM] = {0};

    // Read temperature of all the 64 pixels
    sensor.read_pixel_temperature(pixels_temp);
    // Print 8x8 pixels temperature value in 8 rows by 8 columns
    Serial.println("Temperature for 8x8 matrix are:");
    
    for (int i = 0; i < PIXEL_NUM; i++) {
        Serial.print(pixels_temp[i]);
        Serial.print("  ");
        if (0 == (i + 1) % 8) {
            Serial.println(" ");
        }
    }
    Serial.println(" ");
    Serial.println(" ");
    delay(500);
}

