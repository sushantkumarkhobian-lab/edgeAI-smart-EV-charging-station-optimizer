
#include "State.h"

// Bay status
String bayStatus = "FREE";

// Sensor readings
float voltage = 0.0;
float current = 0.0;
float power = 0.0;
float energyWh = 0.0;
float temperature = 0.0;

// Edge AI
int lastHourOfDay = 12;
unsigned long sessionStartMs = 0;

float predictedArrivalProb = 0.0;
int predictedDurationMin = 0;

// Optimization
bool overloadActive = false;
String loadDecision = "ALLOW";
int throttleLevel = 100;

// RPC manual override
bool manualOverrideActive = false;

// ThingsBoard shared attributes

