#ifndef FUZZY_LOGIC_H
#define FUZZY_LOGIC_H

#include "SystemState.h"

struct SystemDecisions {
    char output_label_light[50];
    float output_value_light;
    char output_label_water[50];
    float output_value_water;
    char output_label_heat[50];
    float output_value_heat;
    char output_label_shadow[50];
    float output_value_shadow;
    char output_label_cooling[50];
    float output_value_cooling;
};

class FuzzyLogic {
public:
    FuzzyLogic();
    
    // Main processing function
    SystemFuzzyState calculateMemberships(const SensorReadings& sensors);
    FuzzyResults calculateFuzzyResults(const SystemFuzzyState& state);
    SystemDecisions defuzzify(FuzzyResults fuzzy_results);
    SystemDecisions evaluateRules(const SystemFuzzyState& state);

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

    SicaklikEtiket string_to_enum_sicaklik(String etiket);
    NemEtiket string_to_enum_nem(String etiket);
    IsikEtiket string_to_enum_isik(String etiket);
    NemToprakEtiket string_to_enum_toprak(String etiket);

    float min(float a, float b);
    float max(float a, float b);

    void get_primary_secondary(FuzzyResult *result, float cd, float d, float o, float y, float cy);

    void evaluate_rules_temp_light_shadow(const char *SicaklikSozel,const char *IsikSozel);
    void evaluate_rules_temp_hum_cooling(const char *SicaklikSozel,const char *HavaNemiSozel);
    void evaluate_rules_temp_hum_heating(const char *SicaklikSozel,const char *HavaNemiSozel);
    void evaluate_rules_hum_soil(char *humidity_label, char *soil_label);
    void evaluate_rules_light(char *light_label, SystemDecisions *system_decisions);

    float GolgelemeDurulastirmaCokDusuk (float x);
    float GolgelemeDurulastirmaDusuk (float x);
    float GolgelemeDurulastirmaOrta (float x);
    float GolgelemeDurulastirmaYuksek (float x);
    float GolgelemeDurulastirmaCokYuksek (float x);

    float SogutmaDurulastirmaCokDusuk (float x);
    float SogutmaDurulastirmaDusuk (float x);
    float SogutmaDurulastirmaOrta (float x);
    float SogutmaDurulastirmaYuksek (float x);
    float SogutmaDurulastirmaCokYuksek (float x);

    float SicaklikDurulastirmaCokDusuk (float x);
    float SicaklikDurulastirmaDusuk (float x);
    float SicaklikDurulastirmaOrta (float x);
    float SicaklikDurulastirmaYuksek (float x);
    float SicaklikDurulastirmaCokYuksek (float x);


    float SulamaDurulastirmaCokDusuk (float x);
    float SulamaDurulastirmaDusuk (float x);
    float SulamaDurulastirmaOrta (float x);
    float SulamaDurulastirmaYuksek (float x);
    float SulamaDurulastirmaCokYuksek (float x);

    float IsiklandirmaDurulastirmaCokDusuk (float x);
    float IsiklandirmaDurulastirmaDusuk (float x);
    float IsiklandirmaDurulastirmaOrta (float x);
    float IsiklandirmaDurulastirmaYuksek (float x);
    float IsiklandirmaDurulastirmaCokYuksek (float x);

    float shadow_defuzzy(float x,float y,float z,float t);
    float cooling_defuzzy(float x,float y,float z,float t);
    float heat_defuzzy(float x,float y,float z,float t);
    float water_defuzzy(float x,float y,float z,float t);
    float light_defuzzy(float primary_mu_light, char* primary_label_light);
    
    // Helper to print single membership
    void printSingleResult(const char* title, float inputVal, const FuzzyMembership& mem);
};

#endif // FUZZY_LOGIC_H
