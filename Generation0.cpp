#include <Arduino.h>
#include "config.h"
#include "Generation0.h"

void initState() {
    digitalWrite(LG1, HIGH);
    digitalWrite(LY1, LOW);
    digitalWrite(LR1, LOW);

    digitalWrite(LG2, LOW);
    digitalWrite(LY2, LOW);
    digitalWrite(LR2, HIGH);
}

void controlGeneration0() {
    static unsigned long previousMillis = 0;
    const long interval = 10000;

    initState();

    if (millis() - previousMillis >= interval) {
      digitalWrite(LG1, !digitalRead(LG1));
      digitalWrite(LY1, !digitalRead(LY1));
      digitalWrite(LR2, !digitalRead(LR2));
      digitalWrite(LY2, !digitalRead(LY2));
      delay(3000);
      digitalWrite(LY1, !digitalRead(LY1));
      digitalWrite(LR1, !digitalRead(LR1));
      digitalWrite(LY2, !digitalRead(LY2));
      digitalWrite(LG2, !digitalRead(LG2));
      delay(10000);
      previousMillis = millis();
    }
}

