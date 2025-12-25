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
// Primary and secondary Membership value and label 

// Container for all fuzzy results
struct SystemFuzzyState {
    FuzzyMembership temp_mu;
    FuzzyMembership hum_mu;
    FuzzyMembership light_mu;
    FuzzyMembership soil_mu;
};

struct FuzzyResult {
    float primary_mu;
    char primary_label[50];
    float secondary_mu;
    char secondary_label[50];
};

struct FuzzyResults {
    FuzzyResult temp_result;
    FuzzyResult hum_result;
    FuzzyResult light_result;
    FuzzyResult soil_result;
};

// Output State
struct SystemOutput {
    float value;
    String label;
};

struct SystemOutputs {
    SystemOutput outputs[5]; // Array for 5 rules/outputs
};

typedef enum
{
    SICAKLIK_LABEL_COK_DUSUK = 0, // 0
    SICAKLIK_LABEL_DUSUK,         // 1
    SICAKLIK_LABEL_ORTA,          // 2
    SICAKLIK_LABEL_YUKSEK,        // 3
    SICAKLIK_LABEL_COK_YUKSEK = 4 // 4
} SicaklikEtiket;

typedef enum
{
    NEM_LABEL_COK_DUSUK = 0, // 0
    NEM_LABEL_DUSUK,         // 1
    NEM_LABEL_ORTA,          // 2
    NEM_LABEL_YUKSEK,        // 3
    NEM_LABEL_COK_YUKSEK = 4 // 4
} NemEtiket;

typedef enum
{
    NEMTOPRAK_LABEL_COK_DUSUK = 0, // 0
    NEMTOPRAK_LABEL_DUSUK,         // 1
    NEMTOPRAK_LABEL_ORTA,          // 2
    NEMTOPRAK_LABEL_YUKSEK,        // 3
    NEMTOPRAK_LABEL_COK_YUKSEK = 4 // 4
} NemToprakEtiket;

typedef enum
{
    ISIK_LABEL_COK_DUSUK = 0, // 0
    ISIK_LABEL_DUSUK,         // 1
    ISIK_LABEL_ORTA,          // 2
    ISIK_LABEL_YUKSEK,        // 3
    ISIK_LABEL_COK_YUKSEK = 4 // 4
} IsikEtiket;

#endif // SYSTEM_STATE_H
