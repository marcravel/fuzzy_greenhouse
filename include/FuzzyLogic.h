#ifndef FUZZY_LOGIC_H
#define FUZZY_LOGIC_H

#include "SystemState.h"

struct SystemDecisions {
    String heating;  // Sicaklik kurali cikisi (cikis)
    String cooling;  // Sogutma kurali cikisi (Cooling)
    String shadow;   // Golgeleme kurali cikisi (shadow)
    String water;    // Sulama kurali cikisi (water)
    String lighting; // Aydinlatma kurali cikisi (lighting)
};

class FuzzyLogic {
public:
    FuzzyLogic();
    
    // Main processing function
    SystemFuzzyState calculateMemberships(const SensorReadings& sensors);
    SystemDecisions evaluateRules(const SystemFuzzyState& state);
    
    // Helpers for debugging/printing
    void printResults(const SensorReadings& inputs, const SystemFuzzyState& state);

private:
    // Math helpers (Membership Functions)
    float trapmf_dusen(float x, float a, float b, float c, float d);
    float trapmf_yukselen(float x, float a, float b, float c, float d);
    float trimf(float x, float a, float b, float c);

    // Specific fuzzifiers
    String getBestLabel(float cok_dusuk, float dusuk, float orta, float yuksek, float cok_yuksek);
    FuzzyMembership fuzzifyTemperature(float temp);
    FuzzyMembership fuzzifyHumidity(float hum);
    FuzzyMembership fuzzifyLight(float light);
    FuzzyMembership fuzzifySoilMoisture(float soil);

    SicaklikEtiket string_to_enum_sicaklik(String etiket);
    NemEtiket string_to_enum_nem(String etiket);
    IsikEtiket string_to_enum_isik(String etiket);
    NemToprakEtiket string_to_enum_toprak(String etiket);
    
    // Helper to print single membership
    void printSingleResult(const char* title, float inputVal, const FuzzyMembership& mem);
};

#endif // FUZZY_LOGIC_H
