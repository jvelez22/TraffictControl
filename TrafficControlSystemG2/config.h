#ifndef CONFIG_H
#define CONFIG_H

#define LDR1 A0 // LDR Light sensor from traffic light 1 connected in pin A0
#define LDR2 A1 // LDR Light sensor from traffic light 2 connected in pin A1
#define CO2  A2 // CO2 sensor connected in pin A3
#define P1 37   // Traffic light 1 button connected in pin 37
#define P2 36   // Traffic light 2 button connected in pin 36
#define CNY1 35 // Infrared sensor 1 in traffic light 1 connected in pin 35
#define CNY2 34 // Infrared sensor 2 in traffic light 1 connected in pin 34
#define CNY3 33 // Infrared sensor 3 in traffic light 1 connected in pin 33
#define CNY4 32 // Infrared sensor 4 in traffic light 2 connected in pin 32
#define CNY5 31 // Infrared sensor 5 in traffic light 2 connected in pin 31
#define CNY6 30 // Infrared sensor 6 in traffic light 2 connected in pin 30
#define LR1 22  // Red traffic light 1 connected in pin 22
#define LY1 23  // Yellow traffic light 1 connected in pin 23
#define LG1 24  // Green traffic light 1 connected in pin 24
#define LR2 25  // Red traffic light 2 connected in pin 25
#define LY2 26  // Yellow traffic light 2 connected in pin 26
#define LG2 27  // Green traffic light 2 connected in pin 27

void setupPins();
void initialState();
void stateOne();
void stateTwo();
void stateThree();

#endif