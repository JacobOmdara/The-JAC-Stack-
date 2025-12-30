#include <Arduino.h>
#include <Servo.h>
#include "stm32f4xx_hal.h"

// Cannot upload correctly in VS Code, use Arduino IDE to upload

Servo myservo;
int ledPin = -1;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  delay(100);

#if defined(PA6)
  myservo.attach(PA6);
#elif defined(A6)
  myservo.attach(A6);
#else
  // Fallback: try digital pin 6 if symbolic names aren't available
  myservo.attach(6);
#endif

  Serial.println("Servo test on PA6 - sweeping 0..180");

  // Configure LED pin (PA7) if available
#if defined(PA7)
  ledPin = PA7;
#elif defined(A7)
  ledPin = A7;
#else
  // Fallback: try digital pin 7 if symbolic names aren't available
  ledPin = 7;
#endif
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Sweep 0 -> 180
  for (int pos = 0; pos <= 180; pos++) {
    myservo.write(pos);
    Serial.print("Angle: ");
    Serial.println(pos);
    // Turn LED ON while sweeping forward
    if (ledPin >= 0) digitalWrite(ledPin, LOW);
    delay(15);
  }
  delay(500);

  // Sweep 180 -> 0
  for (int pos = 180; pos >= 0; pos--) {
    myservo.write(pos);
    Serial.print("Angle: ");
    Serial.println(pos);
    // Turn LED OFF while sweeping reverse
    if (ledPin >= 0) digitalWrite(ledPin, HIGH);
    delay(15);
  }

  delay(1000);
}