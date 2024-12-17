/*
    ADXL345 Triple Axis Accelerometer. Pitch & Roll calculation
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-akcelerometr-adxl345.html
    GIT: https://github.com/jarzebski/Arduino-ADXL345
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
    Modified to replace Lowpass Filter with Kalman Filter
    (c) 2024 by Dr Yeoh Wui Keat
*/

#include <Wire.h>
#include "rgb_lcd.h"
#include "ADXL345.h"
#include <SimpleKalmanFilter.h>

rgb_lcd lcd;
ADXL345 accelerometer;

/*
SimpleKalmanFilter(e_mea, e_est, q)
e_mea: Measurement Uncertainty - Measurement Change Value limit (Filter to track measurement uncertainty until this programmed limit) 
e_est: Estimation Uncertainty  - Replace with a default value (or calculated estimated value) when Measurement Change Value exceeds programmed limit
q: Process Noise               - Known or estimated Gaussian noise level
*/

// For all 3 Kalman filters: 
// Track Measurement (Accelerometer Axis) Change until 0.001, and replace with 0.01 (or calculated estimated value) if Measurement Change exceeds 0.001
SimpleKalmanFilter KalmanFilterX(0.001, 0.1, 0.01);
SimpleKalmanFilter KalmanFilterY(0.001, 0.1, 0.01);
SimpleKalmanFilter KalmanFilterZ(0.001, 0.1, 0.01);

void setup(void) 
{
  Serial.begin(115200);
  lcd.begin(16, 2);                               // Init LCD
  lcd.setRGB(0, 255, 0);                          // Green LCD Back Light

  // Initialize ADXL345
  Serial.println("Initialize ADXL345");

  if (!accelerometer.begin())
  {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    lcd.setRGB(255, 0, 0);                      // Red LCD Back Light
    lcd.print("ACC FAILED!");                   // Display message on LCD
    while(1);                                   // Wait forever.
  }

  // Set measurement range
  // +/-  2G: ADXL345_RANGE_2G
  // +/-  4G: ADXL345_RANGE_4G
  // +/-  8G: ADXL345_RANGE_8G
  // +/- 16G: ADXL345_RANGE_16G
  accelerometer.setRange(ADXL345_RANGE_16G);
}

void loop(void) 
{
  Vector filtered;
  // Read normalized accelerometer values
  Vector norm = accelerometer.readNormalize();

  // Apply Kalman Filter to normalized accelerometer values
  filtered.XAxis = KalmanFilterX.updateEstimate(norm.XAxis);
  filtered.YAxis = KalmanFilterY.updateEstimate(norm.YAxis);
  filtered.ZAxis = KalmanFilterZ.updateEstimate(norm.ZAxis);
  
  // Calculate Pitch & Roll
  int pitch = -(atan2(norm.XAxis, sqrt(norm.YAxis*norm.YAxis + norm.ZAxis*norm.ZAxis))*180.0)/M_PI;
  int roll  =  (atan2(norm.YAxis, norm.ZAxis)*180.0)/M_PI;

  // Calculate Pitch & Roll (Kalman Filter)
  int fpitch = -(atan2(filtered.XAxis, sqrt(filtered.YAxis*filtered.YAxis + filtered.ZAxis*filtered.ZAxis))*180.0)/M_PI;
  int froll  =  (atan2(filtered.YAxis, filtered.ZAxis)*180.0)/M_PI;

  // Output to Serial Plotter (or equivalent)
  Serial.print(pitch);
  Serial.print(" ");
  Serial.print(fpitch);
  Serial.print(" ");
  Serial.print(roll);
  Serial.print(" ");
  Serial.println(froll);

  // Output to LCD
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("P: ["); lcd.print(pitch); lcd.print("]["); lcd.print(fpitch); lcd.print("]");
  lcd.setCursor(0, 1); lcd.print("R: ["); lcd.print(roll);  lcd.print("]["); lcd.print(froll);  lcd.print("]");
  delay(100);
}
