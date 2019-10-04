#include <Stepper.h>
#define SENSOR_PIN A5

// initialize the stepper library on pins 8 through 11
const int stepsPerRevolution = 250;
Stepper motor(stepsPerRevolution, 9, 10, 11, 12);

int motorSpeed = 0;

void setup() {
  // Set the motor speed
  motor.setSpeed(100);
  // Initialize the serial port
  Serial.begin(9600); 
}

void loop() {
  // read the sensor value:
  int motorSpeed = map(analogRead(A5), 0, 1023, 0, 100);
  // set the motor speed:
  if (motorSpeed > 0) {
    motor.setSpeed(motorSpeed);
    // step 1/100 of a revolution:
    motor.step(stepsPerRevolution / 100);
  }
}
