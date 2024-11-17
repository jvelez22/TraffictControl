#include <Arduino.h>
#include <PID_v1_bc.h>
#include "config.h"
#include "Generation2.h"

// Variables PID
double setpoint = 400;
double input, output;
double Kp = 2.0, Ki = 0.5, Kd = 0.1;
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

void setupGeneration2() {
  myPID.SetMode(AUTOMATIC);
}

void controlGeneration2() {
  input = analogRead(CO2);
  myPID.Compute();
  int delayTime = 5000 + output;

  int vehicle1 = digitalRead(CNY1);
  int vehicle2 = digitalRead(CNY2);

  if (vehicle1) {
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    delay(delayTime);
  } else if (vehicle2) {
    digitalWrite(11, HIGH);
    digitalWrite(8, HIGH);
    delay(delayTime);
  }
}
