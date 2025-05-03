#include <Stepper.h>

#define STEPS 200
Stepper irStepper(STEPS, 8, 9, 10, 11);     // IR scanner stepper
Stepper bodyStepper(STEPS, 4, 5, 6, 7);      // Body rotator stepper

const int irSensorPin = 2;     // IR digital out pin
const int armMotorPin1 = A0;   // TT arm motor
const int armMotorPin2 = A1;

const int wheelL1 = 3;  // L298N inputs for left wheel
const int wheelL2 = 12;
const int wheelR1 = 13; // L298N inputs for right wheel
const int wheelR2 = A2;

bool signalDetected = false;

void setup() {
  pinMode(irSensorPin, INPUT);
  pinMode(armMotorPin1, OUTPUT);
  pinMode(armMotorPin2, OUTPUT);
  pinMode(wheelL1, OUTPUT);
  pinMode(wheelL2, OUTPUT);
  pinMode(wheelR1, OUTPUT);
  pinMode(wheelR2, OUTPUT);
  
  irStepper.setSpeed(30);
  bodyStepper.setSpeed(30);
}

void loop() {
  if (!signalDetected) {
    for (int i = 0; i < 100; i++) {
      irStepper.step(1);
      if (digitalRead(irSensorPin) == HIGH) {
        signalDetected = true;
        break;
      }
      delay(10);
    }
    if (!signalDetected) {
      for (int i = 0; i < 100; i++) {
        irStepper.step(-1);
        if (digitalRead(irSensorPin) == HIGH) {
          signalDetected = true;
          break;
        }
        delay(10);
      }
    }
  } else {
    // Step 1: Lower arms
    digitalWrite(armMotorPin1, HIGH);
    digitalWrite(armMotorPin2, LOW);
    delay(1000); // adjust timing as per speed
    digitalWrite(armMotorPin1, LOW);
    digitalWrite(armMotorPin2, LOW);

    // Step 2: Rotate body
    bodyStepper.step(100);  // adjust steps based on angle

    // Step 3: Raise arms
    digitalWrite(armMotorPin1, LOW);
    digitalWrite(armMotorPin2, HIGH);
    delay(1000);
    digitalWrite(armMotorPin1, LOW);
    digitalWrite(armMotorPin2, LOW);

    // Step 4: Move forward
    digitalWrite(wheelL1, HIGH);
    digitalWrite(wheelL2, LOW);
    digitalWrite(wheelR1, HIGH);
    digitalWrite(wheelR2, LOW);
    delay(2000);  // Move for 2 seconds

    // Stop wheels
    digitalWrite(wheelL1, LOW);
    digitalWrite(wheelL2, LOW);
    digitalWrite(wheelR1, LOW);
    digitalWrite(wheelR2, LOW);

    // Reset
    signalDetected = false;
  }
}
