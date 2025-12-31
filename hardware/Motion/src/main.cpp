#include <Arduino.h>
#include <Servo.h>
#include "stm32f4xx_hal.h"

// Cannot upload correctly in VS Code, use Arduino IDE to upload

void test_individual();
void synchronous();
void left();
void right();
void leftToRight();
void rightToLeft();
void leftWave();
void rightWave();

Servo servoLeft;
Servo servoRight;

int ledRed;
int ledGreen;
int ledBlue;
// if left empty during testing, it will default to ON
int leftInput;
int rightInput;
int waveInput;
int bothInput;

int timeSpeed = 10;
int degreesShift = 60;

int leftState;
int rightState;
int waveState;
int bothState;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  delay(100);

  servoLeft.attach(PE11);
  servoRight.attach(PE13);
  /* PWM compatible pins: {
                            PA0, PZ1, PA2, PA3, PA6, PA7, PA8, PA9, PA10, PA11
                            PB0, PB1, PB3, PB6, PB7, PB8, PB9, PB10, PB11
                            PC6, PC7, PC8, PC9
                            PD12, PD13, PD14, PD15
                            PE5, PE6, PE9, PE11, PE13, PE14
                            
                            } */

  Serial.println("Servo test on PA6 - sweeping 0..180");

  ledRed = PE0;
  ledGreen = PE1;
  ledBlue = PB9;

  leftInput = PE2;
  rightInput = PE3;
  waveInput = PE4;
  bothInput = PE5;

  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  pinMode(leftInput, INPUT_PULLUP);
  pinMode(rightInput, INPUT_PULLUP);
  pinMode(waveInput, INPUT_PULLUP);
  pinMode(bothInput, INPUT_PULLUP);

  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);

  servoLeft.write(0);
  servoRight.write(180);
}

void loop() {
  delay(1000);

  leftState = digitalRead(leftInput);
  rightState = digitalRead(rightInput);
  waveState = digitalRead(waveInput);
  bothState = digitalRead(bothInput);

  if (leftState && rightState) {
    synchronous();
  } else if (leftState) {
    if (bothState && !waveState) {
      leftToRight();
    } else if (!bothState && waveState) {
      leftWave();
    } else {
      left();
    }
  } else if (rightState) {
    if (bothState && !waveState) {
      rightToLeft();
    } else if (!bothState && waveState) {
      rightWave();
    } else {
      right();
    }
  }
}


// ----------------------------------------------------

void test_individual() {
  // Left
  left();

  // Right
  right();
}

void synchronous() {
  digitalWrite(ledGreen, HIGH);
  for (int pos = 0; pos <= 180; pos++) {
    servoLeft.write(pos);
    servoRight.write(180-pos);
    delay(timeSpeed);
  }

  for (int pos = 180; pos >= 0; pos --) {
    servoLeft.write(pos);
    servoRight.write(180-pos);
    delay(timeSpeed);
  }

  digitalWrite(ledGreen, LOW);
}

void left() {
  digitalWrite(ledRed, HIGH);

  // Sweep 0 -> 180
  for (int pos = 0; pos <= 180; pos++) {
    servoLeft.write(pos);
    delay(timeSpeed);
  }
  delay(500);

  // Sweep 180 -> 0
  for (int pos = 180; pos >= 0; pos--) {
    servoLeft.write(pos);
    delay(timeSpeed);
  }

  digitalWrite(ledRed, LOW);
}

void right() {
  digitalWrite(ledBlue, HIGH);

  // Sweep 0 -> 180
  for (int pos = 180; pos >= 0; pos--) {
    servoRight.write(pos);
    delay(timeSpeed);
  }
  delay(500);

  // Sweep 180 -> 0
  for (int pos = 0; pos <= 180; pos++) {
    servoRight.write(pos);
    delay(timeSpeed);
  }

  digitalWrite(ledBlue, LOW);
}

void leftToRight() {
  for (int count = 0; count <= 360 + degreesShift; count++) {
    if (count <= 180) {
      servoLeft.write(count);
      digitalWrite(ledRed, HIGH);
    } else {
      servoLeft.write(360-count);
      digitalWrite(ledRed, LOW);
    }
    
    if (count <= 180 + degreesShift) {
      servoRight.write(180+degreesShift-count);
      if (count >= degreesShift) {
        digitalWrite(ledBlue, HIGH);
      }
    } else {
      servoRight.write(count-(180+degreesShift));
      digitalWrite(ledBlue, LOW);
    }
    delay(timeSpeed);
  }
}

void rightToLeft() {
  for (int count = 0; count <= 360 + degreesShift; count++) {
    if (count <= 180 + degreesShift) {
      servoLeft.write(count-degreesShift);
      if (count >= degreesShift) {
        digitalWrite(ledRed, HIGH);
      }
    } else {
      servoLeft.write(360-count+degreesShift);
      digitalWrite(ledRed, LOW);
    }
    
    if (count <= 180) {
      servoRight.write(180-count);
      digitalWrite(ledBlue, HIGH);
    } else {
      servoRight.write(count-180);
      digitalWrite(ledBlue, LOW);
    }
    delay(timeSpeed);
  }
}

void leftWave() {
  digitalWrite(ledRed, HIGH);

  // Raise hand
  for (int pos = 0; pos <= 180; pos++) {
    servoLeft.write(pos);
    delay(int(timeSpeed/2));
  }

  delay(timeSpeed*10);

  // Wave hand
  for (int i = 0; i < 2; i++) {
    for (int pos = 180; pos >= 150; pos--) {
      servoLeft.write(pos);
      delay(timeSpeed);
    }

    for (int pos = 150; pos <= 180; pos++) {
      servoLeft.write(pos);
      delay(timeSpeed);
    }
  }

  delay(timeSpeed*20);

  // Lower hand
  for (int pos = 180; pos >= 0; pos--) {
    servoLeft.write(pos);
    delay(int(timeSpeed/2));
  }

  digitalWrite(ledRed, LOW);
}

void rightWave() {
  digitalWrite(ledBlue, HIGH);

  // Raise hand
  for (int pos = 180; pos >= 0; pos--) {
    servoRight.write(pos);
    delay(int(timeSpeed/2));
  }

  delay(timeSpeed*10);

  // Wave hand
  for (int i = 0; i < 2; i++) {
    for (int pos = 0; pos <= 30; pos++) {
      servoRight.write(pos);
      delay(timeSpeed);
    }

    for (int pos = 30; pos >= 0; pos--) {
      servoRight.write(pos);
      delay(timeSpeed);
    }
  }

  delay(timeSpeed*20);

  // Lower hand
  for (int pos = 0; pos <= 180; pos++) {
    servoRight.write(pos);
    delay(int(timeSpeed/2));
  }

  digitalWrite(ledBlue, LOW);
}