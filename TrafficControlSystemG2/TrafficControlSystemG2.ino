#include <LiquidCrystal_I2C.h>
#include "config.h"

const int POPULATION_SIZE = 6;
const int MUTATION_RATE = 10;
const int GENERATIONS = 5;

LiquidCrystal_I2C lcd(0x27, 16, 4);

struct Chromosome {
  int times[6];
  int fitness;
};

Chromosome population[POPULATION_SIZE];
Chromosome bestChromosome;

int evaluateFitness(Chromosome c) {
  int vehicleCount1 = digitalRead(CNY1) + digitalRead(CNY2) + digitalRead(CNY3);
  int vehicleCount2 = digitalRead(CNY4) + digitalRead(CNY5) + digitalRead(CNY6);
  int co2Level = analogRead(CO2);

  int time1 = c.times[0];
  int time2 = c.times[1];

  int fitness1 = (vehicleCount1 * 10 + co2Level * 5) - abs(vehicleCount1 - time1);
  int fitness2 = (vehicleCount2 * 10) - abs(vehicleCount2 - time2);

  return fitness1 + fitness2;
}

Chromosome getBestChromosome() {
  Chromosome best = population[0];
  for (int i = 1; i < POPULATION_SIZE; i++) {
    if (population[i].fitness > best.fitness) {
      best = population[i];
    }
  }
  return best;
}

void evolvePopulation() {
  for (int i = 0; i < POPULATION_SIZE; i++) {
    if (random(100) < MUTATION_RATE) {
      int gene = random(6);

      // Leer tráfico y CO₂ en tiempo real
      int vehicleCount1 = digitalRead(CNY1) + digitalRead(CNY2) + digitalRead(CNY3);
      int vehicleCount2 = digitalRead(CNY4) + digitalRead(CNY5) + digitalRead(CNY6);
      int co2Level = analogRead(CO2);

      Serial.print("vehicleCount1: ");
      Serial.println(vehicleCount1);

      Serial.print("vehicleCount2: ");
      Serial.println(vehicleCount2);

      // Ajustar mutación según tráfico y CO₂
      if (gene == 0 || gene == 1) { // Tiempos relacionados con ruta 1
        population[i].times[gene] = max(5, min(20, vehicleCount1 + co2Level / 10));
      } else if (gene == 2 || gene == 3) { // Tiempos relacionados con ruta 2
        population[i].times[gene] = max(5, min(20, vehicleCount2));
      } else {
        // Genes de otras fases: mantener mutación aleatoria controlada
        population[i].times[gene] = random(5, 20);
      }
    }
    // Recalcular la aptitud del cromosoma después de la mutación
    population[i].fitness = evaluateFitness(population[i]);
  }
}


void controlTrafficLights(Chromosome c) {
  int vehicleCount1 = digitalRead(CNY1) + digitalRead(CNY2) + digitalRead(CNY3);
  int vehicleCount2 = digitalRead(CNY4) + digitalRead(CNY5) + digitalRead(CNY6);
  int co2Level = analogRead(CO2);
  int totalVehicles = vehicleCount1 + vehicleCount2;

  int weight1 = vehicleCount1 + co2Level / 10;
  int weight2 = vehicleCount2;

  int time1 = (weight1 * c.times[0]) / max(1, weight1 + weight2);
  int time2 = (weight2 * c.times[1]) / max(1, weight1 + weight2);

  stateOne();
  delay(3000);

  stateTwo();
  delay(time1 * 1000);

  stateThree();
  delay(3000);

  initialState();
  delay(time2 * 1000);

  digitalWrite(LY2, LOW);
  digitalWrite(LR2, HIGH);
}

void setupLcd() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema iniciado");
}

void setupGeneration2() {
  for (int i = 0; i < POPULATION_SIZE; i++) {
    for (int j = 0; j < 6; j++) {
      population[i].times[j] = random(5, 20);
    }
    population[i].fitness = evaluateFitness(population[i]);
  }
  bestChromosome = getBestChromosome();
}

void controlGeneration2() {
  for (int gen = 0; gen < GENERATIONS; gen++) {
    evolvePopulation();
  }

  bestChromosome = getBestChromosome();
  controlTrafficLights(bestChromosome);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO2: ");
  lcd.print(analogRead(CO2));
  lcd.setCursor(0, 1);
  lcd.print("Calle 1: ");
  lcd.print(bestChromosome.times[0]);
  lcd.print("s V");
  lcd.setCursor(0, 2);
  lcd.print("Calle 2: ");
  lcd.print(bestChromosome.times[3]);
  lcd.print("s V");
  Serial.println("{\"CO2\": "+ String(analogRead(CO2)) +", \"Time1\": "+String(bestChromosome.times[0])+", \"Time2\": "+String(bestChromosome.times[3])+"}");
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  setupPins();
  setupLcd();
  initialState();
  setupGeneration2();
}

void loop() {
  controlGeneration2();
}
