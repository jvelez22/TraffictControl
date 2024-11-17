#include "HardwareSerial.h"
#include <Arduino.h>
#include "config.h"
#include "Generation1.h"

const int baseTimeGreen = 2000;
const int additionalTimePerVehicle = 2000;

void controlGeneration1() {
  int vehicleRoute1 = digitalRead(CNY1) + digitalRead(CNY2) + digitalRead(CNY3);
  int vehicleRoute2 = digitalRead(CNY4) + digitalRead(CNY5) + digitalRead(CNY6);

  Serial.print("vehicleRoute1: ");
  Serial.println(vehicleRoute1);
  Serial.println("-------------------------------------");
  Serial.print("vehicleRoute2: ");
  Serial.println(vehicleRoute2);
  
  int greenTime = baseTimeGreen + (vehicleRoute1 * additionalTimePerVehicle);
  int greenTime2 = baseTimeGreen + (vehicleRoute2 * additionalTimePerVehicle);

  digitalWrite(LG1, HIGH);
  digitalWrite(LR2, HIGH);
  delay(greenTime);

  digitalWrite(LG1, LOW);
  digitalWrite(LR1, HIGH);
  digitalWrite(LR2, LOW);
  digitalWrite(LG2, HIGH);
  delay(greenTime2);

  digitalWrite(LG2, LOW);
  digitalWrite(LR1, LOW);
}
