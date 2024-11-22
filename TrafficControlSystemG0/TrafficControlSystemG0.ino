#include <LiquidCrystal_I2C.h>
#include "config.h"

LiquidCrystal_I2C lcd(0x27, 16, 4);

void setupLcd() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema iniciado");
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

void controlGeneration0() {
  static unsigned long previousMillis = 0;
  const long interval = 10000;

  if (millis() - previousMillis >= interval) {
    stateOne();
    delay(3000);
    stateTwo();
    delay(10000);
    stateThree()
    delay(3000);
    initialState();
    delay(10000);
    previousMillis = millis();
  }
}

void setup() {
  Serial.begin(9600);
  setupPins();
  setupLcd();
  initialState();
}

void loop() {
    controlGeneration0();
}