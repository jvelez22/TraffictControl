#include <Arduino.h>
#include "config.h"
#include "Generation1.h"

void controlGeneration1() {
  int vehicle1 = digitalRead(CNY1);
  int vehicle2 = digitalRead(CNY2);
  int lightLevel = analogRead(LDR1);

  int brightness = (lightLevel < 500) ? 255 : 128;

  if (vehicle1) {
    analogWrite(9, brightness);
    digitalWrite(10, HIGH);
    delay(5000);
  } else if (vehicle2) {
    analogWrite(11, brightness);
    digitalWrite(8, HIGH);
    delay(5000);
  }
}
