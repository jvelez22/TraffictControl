#include "Generation0.h"
#include "Generation1.h"
#include "Generation2.h"
#include "config.h"

#define MODE_SWITCH_PIN 2

int currentMode = 0;

void setup() {
  pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  setupPins();
  setupGeneration2();
}

void loop() {
  if (digitalRead(MODE_SWITCH_PIN) == LOW) {
    currentMode = (currentMode + 1) % 3;
    delay(500);
  }

  switch (currentMode) {
    case 0:
      controlGeneration0();
      break;
    case 1:
      controlGeneration1();
      break;
    case 2:
      controlGeneration2();
      break;
  }
}