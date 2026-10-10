
#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

// Bay status
extern String bayStatus;

// Sensor readings
extern float voltage;
extern float current;
extern float power;
extern float energyWh;
extern float temperature;

// Edge AI variables
extern int lastHourOfDay;
extern unsigned long sessionStartMs;

extern float predictedArrivalProb;
extern int predictedDurationMin;

// Optimization variables
extern bool overloadActive;
extern String loadDecision;
extern int throttleLevel;

// RPC manual override
extern bool manualOverrideActive;

#endif
