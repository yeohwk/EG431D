#include <Wire.h>
#include <ADXL345.h>

ADXL345 adxl;   // Initialize ADXL345 instance of the ADXL345 library                     

void setup() {
    Serial.begin(9600);
    adxl.powerOn();

    // Set activity/inactivity thresholds (0-255)
    adxl.setActivityThreshold(75);    // 62.5mg per increment
    adxl.setInactivityThreshold(75);  // 62.5mg per increment
    adxl.setTimeInactivity(10);       // Set inactive timeout

    // Monitoring of activity movement on these axes -> 1 = on; 0 = off
    adxl.setActivityX(1);
    adxl.setActivityY(1);
    adxl.setActivityZ(1);

    // Monitoring of inactivity movement on these axes -> 1 = on; 0 = off
    adxl.setInactivityX(1);
    adxl.setInactivityY(1);
    adxl.setInactivityZ(1);

    // Monitoring of tap movement on these axes -> 1 = on; 0 = off
    adxl.setTapDetectionOnX(0);
    adxl.setTapDetectionOnY(0);
    adxl.setTapDetectionOnZ(1);

    // Set values for tap, and double-tap (0-255)
    adxl.setTapThreshold(50);     //62.5mg per increment
    adxl.setTapDuration(15);      //625us per increment
    adxl.setDoubleTapLatency(80); //1.25ms per increment
    adxl.setDoubleTapWindow(200); //1.25ms per increment

    // Set freefall values (0-255)
    adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
    adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment

    // Setting all interrupts to take place on Int pin 1
    // Issue (bug) with Int pin 2 -> unable to reset it
    adxl.setInterruptMapping(ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN);

    // Register interrupt actions - 1 == on; 0 == off
    adxl.setInterrupt(ADXL345_INT_SINGLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_DOUBLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_FREE_FALL_BIT,  1);
    adxl.setInterrupt(ADXL345_INT_ACTIVITY_BIT,   1);
    adxl.setInterrupt(ADXL345_INT_INACTIVITY_BIT, 1);
}

void loop() {
    int x, y, z;
    double xyz[3];
    double ax, ay, az;

    // Read the accelerometer values and store them in variables  x,y,z  
    adxl.readXYZ(&x, &y, &z); 

    // Output x,y,z values
    Serial.print("values of X , Y , Z: ");
    Serial.print(x);
    Serial.print(" , ");
    Serial.print(y);
    Serial.print(" , ");
    Serial.println(z);

    adxl.getAcceleration(xyz);
    ax = xyz[0];
    ay = xyz[1];
    az = xyz[2];
    Serial.print("X=");
    Serial.print(ax);
    Serial.println(" g");
    Serial.print("Y=");
    Serial.print(ay);
    Serial.println(" g");
    Serial.print("Z=");
    Serial.print(az);
    Serial.println(" g");
    Serial.println("**********************");
    delay(500);
}

