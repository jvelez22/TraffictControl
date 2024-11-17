#include <Arduino.h>
#include "config.h"
#include "Generation2.h"

const int POPULATION_SIZE = 6;
const int MUTATION_RATE = 10;
const int GENERATIONS = 5;

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
  
  return 1000 - (vehicleCount1 + vehicleCount2 + co2Level);
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
      population[i].times[gene] = random(5, 20);
    }
    population[i].fitness = evaluateFitness(population[i]);
  }
}

void controlTrafficLights(Chromosome c) {
  digitalWrite(LG1, HIGH); digitalWrite(LR2, HIGH);
  delay(c.times[0] * 1000);
  digitalWrite(LG1, LOW); digitalWrite(LY1, HIGH);
  delay(c.times[1] * 1000);
  digitalWrite(LY1, LOW); digitalWrite(LR1, HIGH);

  digitalWrite(LG2, HIGH); digitalWrite(LR1, HIGH);
  delay(c.times[3] * 1000);
  digitalWrite(LG2, LOW); digitalWrite(LY2, HIGH);
  delay(c.times[4] * 1000);
  digitalWrite(LY2, LOW); digitalWrite(LR2, HIGH);
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

  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("CO2: ");
  // lcd.print(analogRead(co2Sensor));
  // lcd.setCursor(0, 1);
  // lcd.print("Calle 1: ");
  // lcd.print(bestChromosome.times[0]);
  // lcd.print("s V");
  // lcd.setCursor(0, 2);
  // lcd.print("Calle 2: ");
  // lcd.print(bestChromosome.times[3]);
  // lcd.print("s V");
  delay(5000);
}
