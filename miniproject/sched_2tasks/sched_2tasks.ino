/*
2 Simple Tasks

 Demonstrates the use of the Scheduler library for the Arduino MKR1010 WiFi

 Hardware required :
 * LED_BUILTIN and LED connected to pin 4

*/

// Include Scheduler since we want to manage multiple tasks.
#include <Scheduler.h>

#define led1  LED_BUILTIN   // Builtin Amber LED
#define led2  4             // External Red LED

void setup() {
  Serial.begin(115200);

  // Setup the 3 pins as OUTPUT
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Add "loop2" to scheduling.
  // "loop" is always started by default.
  Scheduler.startLoop(loop2);
}

// Task no.1: blink LED with 1 second delay.
void loop() {
  Serial.println("Task1!");
  digitalWrite(led1, HIGH);

  // IMPORTANT:
  // When multiple tasks are running 'delay' passes control to
  // other tasks while waiting and guarantees they get executed.
  delay(1000);

  digitalWrite(led1, LOW);
  delay(1000);
}

// Task no.2: accept commands from Serial port
// '0' turns on LED
// '1' turns off LED
void loop2() {
//  Serial.println("Task2!");
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '0') {
      digitalWrite(led2, LOW);
      Serial.println("Led turned on!");
    }
    if (c == '1') {
      digitalWrite(led2, HIGH);
      Serial.println("Led turned off!");
    }
  }

  // IMPORTANT:
  // We must call 'yield' at a regular basis to pass
  // control to other tasks.
  yield();
}
