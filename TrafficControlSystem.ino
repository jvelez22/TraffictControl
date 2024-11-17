#include <LiquidCrystal_I2C.h>
#include "Generation0.h"
#include "Generation1.h"
#include "Generation2.h"
#include "config.h"

LiquidCrystal_I2C lcd(0x27, 16, 4);
int currentMode = 0;

void setupLcd() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema iniciado");
}

void setup() {
  Serial.begin(9600);
  setupPins();
  setupLcd();
  setupGeneration2();
  delay(1000);
}

void loop() {
  int readButton = digitalRead(P1);
  if (readButton == HIGH) {
    if (currentMode == 2) currentMode = 0;
    currentMode++;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mode: ");
    lcd.print(currentMode);
    delay(500);
  }
  Serial.println(digitalRead(CNY1));

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