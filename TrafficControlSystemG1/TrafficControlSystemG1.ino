#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);

#define CNY1 35
#define CNY2 34
#define CNY3 33
#define CNY4 32
#define CNY5 31
#define CNY6 30
#define CO2_SENSOR A0 
#define LDR1 A0
#define LDR2 A1 
#define LR1 22
#define LY1 23
#define LG1 24
#define LR2 25
#define LY2 26
#define LG2 27

int trafficDensity1 = 0;
int trafficDensity2 = 0;
unsigned long greenTime1 = 5000;
unsigned long greenTime2 = 5000;
unsigned long lastSwitch = 0;
int activeLight = 1; // 1 = semáforo 1 verde, 2 = semáforo 2 verde
int co2Level = 0;
bool flashingMode = false; 
unsigned long lastFlashTime = 0;
bool flashState = false; // luces intermitentes

void setup() {
  pinMode(CNY1, INPUT_PULLUP);
  pinMode(CNY2, INPUT_PULLUP);
  pinMode(CNY3, INPUT_PULLUP);
  pinMode(CNY4, INPUT_PULLUP);
  pinMode(CNY5, INPUT_PULLUP);
  pinMode(CNY6, INPUT_PULLUP);
  pinMode(LR1, OUTPUT);
  pinMode(LY1, OUTPUT);
  pinMode(LG1, OUTPUT);
  pinMode(LR2, OUTPUT);
  pinMode(LY2, OUTPUT);
  pinMode(LG2, OUTPUT);

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  resetLights();
}

void loop() {
  unsigned long currentMillis = millis();

  int light1 = analogRead(LDR1);
  int light2 = analogRead(LDR2);

  // Verificar si el sistema debe entrar en modo intermitente
  if (light1 < 200 && light2 < 200) { // Umbral de oscuridad
    if (!flashingMode) {
      flashingMode = true;
      resetLights(); 
    }
    manageFlashingLights(currentMillis);
    return; // Salir del bucle principal y mantener el modo intermitente
  } else if (flashingMode) {
    flashingMode = false; // Salir del modo intermitente
    resetLights(); // Asegurar que las luces vuelvan al estado normal
  }

  trafficDensity1 = calculateTrafficDensity(CNY1, CNY2, CNY3);
  trafficDensity2 = calculateTrafficDensity(CNY4, CNY5, CNY6);
  co2Level = readCO2();

  greenTime1 = adaptGreenTime(trafficDensity1, co2Level);
  greenTime2 = adaptGreenTime(trafficDensity2, co2Level);

  if (activeLight == 1 && currentMillis - lastSwitch >= greenTime1) {
    switchTrafficLights(1);
    activeLight = 2;
    lastSwitch = currentMillis;
  } else if (activeLight == 2 && currentMillis - lastSwitch >= greenTime2) {
    switchTrafficLights(2); 
    activeLight = 1;
    lastSwitch = currentMillis;
  }

  displayInfo(trafficDensity1, trafficDensity2, greenTime1, greenTime2, co2Level);
  delay(800);
}

//luces intermitentes
void manageFlashingLights(unsigned long currentMillis) {
  if (currentMillis - lastFlashTime >= 500) { // cada 500 ms
    lastFlashTime = currentMillis;
    flashState = !flashState;

    digitalWrite(LY1, flashState);
    digitalWrite(LY2, flashState);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Modo intermitente");
  lcd.setCursor(0, 1);
  lcd.print("Luz baja detectada");
}

// calcular la densidad de tráfico
int calculateTrafficDensity(int sensor1, int sensor2, int sensor3) {
  int s1 = !digitalRead(sensor1);
  int s2 = !digitalRead(sensor2);
  int s3 = !digitalRead(sensor3);
  return s1 + s2 + s3;
}

// leer el nivel de CO2
int readCO2() {
  int co2 = analogRead(CO2_SENSOR);
  Serial.print("Nivel de CO2: ");
  Serial.println(co2);
  return co2;
}

// tiempo verde adaptativo
unsigned long adaptGreenTime(int density, int co2Level) {
  unsigned long baseTime = (density == 0) ? 10000 : map(density, 0, 3, 10000, 20000);

  // Ajustar tiempo basado en nivel de CO2
  if (co2Level < 300) {
    return baseTime; // Niveles bajos: sin cambio
  } else if (co2Level >= 300 && co2Level < 700) {
    return baseTime + 2000; // Niveles moderados: incrementar 2 segundos
  } else {
    return baseTime + 5000; // Niveles altos: incrementar 5 segundos
  }
}

//cambiar los semáforos
void switchTrafficLights(int currentLight) {
  if (currentLight == 1) {
    // Apagar semáforo 1
    digitalWrite(LG1, LOW);
    digitalWrite(LY1, HIGH);
    delay(2000); // Tiempo de luz amarilla
    digitalWrite(LY1, LOW);
    digitalWrite(LR1, HIGH);

    // Encender semáforo 2
    digitalWrite(LR2, LOW);
    digitalWrite(LG2, HIGH);
  } else if (currentLight == 2) {
    // Apagar semáforo 2
    digitalWrite(LG2, LOW);
    digitalWrite(LY2, HIGH);
    delay(2000); // Tiempo de luz amarilla
    digitalWrite(LY2, LOW);
    digitalWrite(LR2, HIGH);

    // Encender semáforo 1
    digitalWrite(LR1, LOW);
    digitalWrite(LG1, HIGH);
  }
}

// mostrar datos en pantalla
void displayInfo(int density1, int density2, unsigned long time1, unsigned long time2, int co2) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("S1 Trafico: ");
  lcd.print(density1);

  lcd.setCursor(0, 1);
  lcd.print("S2 Trafico: ");
  lcd.print(density2);

  lcd.setCursor(0, 2);
  lcd.print("CO2: ");
  lcd.print(co2);

  lcd.setCursor(0, 3);
  lcd.print("S1:");
  lcd.print(time1 / 1000);
  lcd.print("s S2:");
  lcd.print(time2 / 1000);
  lcd.print("s");
}

// apagar todas las luces
void resetLights() {
  digitalWrite(LR1, LOW);
  digitalWrite(LY1, LOW);
  digitalWrite(LG1, LOW);
  digitalWrite(LR2, LOW);
  digitalWrite(LY2, LOW);
  digitalWrite(LG2, LOW);
}
