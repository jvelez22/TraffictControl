#include <Arduino.h>
#include "config.h"

void setupPins() {
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  pinMode(CO2, INPUT);
  pinMode(P1, INPUT_PULLUP);
  pinMode(P2, INPUT_PULLUP);
  pinMode(CNY1, INPUT);
  pinMode(CNY2, INPUT);
  pinMode(CNY3, INPUT);
  pinMode(CNY4, INPUT);
  pinMode(CNY5, INPUT);
  pinMode(CNY6, INPUT);
  pinMode(LR1, OUTPUT);
  pinMode(LY1, OUTPUT);
  pinMode(LG1, OUTPUT);
  pinMode(LR2, OUTPUT);
  pinMode(LY2, OUTPUT);
  pinMode(LG2, OUTPUT);
}

void initialState() {
  digitalWrite(LG1, HIGH);
  digitalWrite(LY1, LOW);
  digitalWrite(LR1, LOW);

  digitalWrite(LG2, LOW);
  digitalWrite(LY2, LOW);
  digitalWrite(LR2, HIGH);
}

void stateOne() {
  digitalWrite(LG1, LOW);
  digitalWrite(LY1, HIGH);
  digitalWrite(LR2, LOW);
  digitalWrite(LY2, HIGH);
}

void stateTwo() {
  digitalWrite(LY1, LOW);
  digitalWrite(LR1, HIGH);
  digitalWrite(LY2, LOW);
  digitalWrite(LG2, HIGH);
}

void stateThree() {
  digitalWrite(LY1, HIGH);
  digitalWrite(LR1, LOW);
  digitalWrite(LY2, HIGH);
  digitalWrite(LG2, LOW);
}
