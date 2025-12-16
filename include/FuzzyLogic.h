#ifndef FUZZY_LOGIC_H
#define FUZZY_LOGIC_H

#include "SystemState.h"

class FuzzyLogic {
public:
    FuzzyLogic();
    
    // Main processing function
    SystemFuzzyState calculateMemberships(const SensorReadings& sensors);
    
    // Helpers for debugging/printing
    void printResults(const SensorReadings& inputs, const SystemFuzzyState& state);

private:
    // Math helpers (Membership Functions)
    float trapmf_dusen(float x, float a, float b, float c, float d);
    float trapmf_yukselen(float x, float a, float b, float c, float d);
    float trimf(float x, float a, float b, float c);

    // Specific fuzzifiers
    FuzzyMembership fuzzifyTemperature(float temp);
    FuzzyMembership fuzzifyHumidity(float hum);
    FuzzyMembership fuzzifyLight(float light);
    FuzzyMembership fuzzifySoilMoisture(float soil);
    
    // Helper to print single membership
    void printSingleResult(const char* title, float inputVal, const FuzzyMembership& mem);
};

#endif // FUZZY_LOGIC_H
