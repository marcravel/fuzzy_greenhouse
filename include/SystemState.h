#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <Arduino.h>

// Sensor Readings Structure
struct SensorReadings {
    float temperature;
    float humidity;
    float lightLevel;
    float soilMoisture;
};

// Fuzzy Logic Membership Results (Turkish naming preserved for domain logic)
struct FuzzyMembership {
    float cok_dusuk;  // Very Low
    float dusuk;      // Low
    float orta;       // Medium
    float yuksek;     // High
    float cok_yuksek; // Very High
};

// Container for all fuzzy results
struct SystemFuzzyState {
    FuzzyMembership temp_mu;
    FuzzyMembership hum_mu;
    FuzzyMembership light_mu;
    FuzzyMembership soil_mu;
};

// Output State
struct SystemOutput {
    float membership;
    float value;
    String label;
};

struct SystemOutputs {
    SystemOutput outputs[5]; // Array for 5 rules/outputs
};

#endif // SYSTEM_STATE_H
