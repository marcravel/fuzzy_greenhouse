#include "FuzzyLogic.h"
#include <math.h>

FuzzyLogic::FuzzyLogic() {}

// --- Math Helpers ---

float FuzzyLogic::trapmf_yukselen(float x, float a, float b, float c, float d) {
    float mu = 0.0f;
    if (b <= x && x <= c) {
        mu = 1.0f;
    } else {
        mu = (x - a) / (b - a);
    }
    if (mu < 0.0f) mu = 0.0f;
    if (mu > 1.0f) mu = 1.0f;
    return mu;
}

float FuzzyLogic::trapmf_dusen(float x, float a, float b, float c, float d) {
    float mu = 0.0f;
    if (a <= x && x <= c) {
        mu = 1.0f;
    } else {
        mu = (d - x) / (d - c);
    }
    if (mu < 0.0f) mu = 0.0f;
    if (mu > 1.0f) mu = 1.0f;
    return mu;
}

float FuzzyLogic::trimf(float x, float a, float b, float c) {
    float mu;
    if (x <= a || x >= c) {
        mu = 0.0f;
    } else if (x >= a && x <= b) {
        mu = (x - a) / (b - a);
    } else if (x >= b && x <= c) {
        mu = (c - x) / (c - b);
    } else {
        mu = 0.0f;
    }
    // simple bounds check equivalent to max(0, min(1, mu))
    if (mu < 0.0f) return 0.0f;
    if (mu > 1.0f) return 1.0f;
    return mu;
}

// --- Fuzzification Logic ---

FuzzyMembership FuzzyLogic::fuzzifyTemperature(float temp) {
    FuzzyMembership mu;
    mu.cok_dusuk  = trapmf_dusen(temp, -10.0f, -10.0f, 0.0f, 10.0f);
    mu.dusuk      = trimf(temp, 0.0f, 7.5f, 15.0f);
    mu.orta       = trimf(temp, 14.0f, 20.0f, 26.0f);
    mu.yuksek     = trimf(temp, 20.0f, 30.0f, 40.0f);
    mu.cok_yuksek = trapmf_yukselen(temp, 30.0f, 40.0f, 50.0f, 50.0f);
    return mu;
}

FuzzyMembership FuzzyLogic::fuzzifyHumidity(float hum) {
    FuzzyMembership mu;
    mu.cok_dusuk  = trapmf_dusen(hum, 0.0f, 0.0f, 20.0f, 40.0f);
    mu.dusuk      = trimf(hum, 20.0f, 39.5f, 59.0f);
    mu.orta       = trimf(hum, 50.0f, 60.0f, 70.0f);
    mu.yuksek     = trimf(hum, 60.0f, 75.0f, 90.0f);
    mu.cok_yuksek = trapmf_yukselen(hum, 80.0f, 90.0f, 100.0f, 100.0f);
    return mu;
}

FuzzyMembership FuzzyLogic::fuzzifyLight(float light) {
    FuzzyMembership mu;
    mu.cok_dusuk  = trapmf_dusen(light, 0.0f, 0.0f, 2500.0f, 5000.0f);
    mu.dusuk      = trimf(light, 4000.0f, 6500.0f, 9000.0f);
    mu.orta       = trimf(light, 8000.0f, 10000.0f, 12000.0f);
    mu.yuksek     = trimf(light, 11000.0f, 13500.0f, 16000.0f);
    mu.cok_yuksek = trapmf_yukselen(light, 15000.0f, 17500.0f, 20000.0f, 20000.0f);
    return mu;
}

FuzzyMembership FuzzyLogic::fuzzifySoilMoisture(float soil) {
    FuzzyMembership mu;
    mu.cok_dusuk  = trapmf_dusen(soil, 0.0f, 0.0f, 20.0f, 40.0f);
    mu.dusuk      = trimf(soil, 30.0f, 49.5f, 69.0f);
    mu.orta       = trimf(soil, 60.0f, 69.5f, 79.0f);
    mu.yuksek     = trimf(soil, 70.0f, 80.0f, 90.0f);
    mu.cok_yuksek = trapmf_yukselen(soil, 80.0f, 90.0f, 100.0f, 100.0f);
    return mu;
}

SystemFuzzyState FuzzyLogic::calculateMemberships(const SensorReadings& sensors) {
    SystemFuzzyState state;
    state.temp_mu = fuzzifyTemperature(sensors.temperature);
    state.hum_mu = fuzzifyHumidity(sensors.humidity);
    state.light_mu = fuzzifyLight(sensors.lightLevel);
    state.soil_mu = fuzzifySoilMoisture(sensors.soilMoisture);
    return state;
}

// --- Debug / Printing ---

void FuzzyLogic::printSingleResult(const char* title, float inputVal, const FuzzyMembership& mem) {
    printf(">>> %s (Giris: %.1f) <<<\n", title, inputVal);
    printf("1. COK DUSUK:  %.4f\n", mem.cok_dusuk);
    printf("2. DUSUK:      %.4f\n", mem.dusuk);
    printf("3. ORTA:       %.4f\n", mem.orta);
    printf("4. YUKSEK:     %.4f\n", mem.yuksek);
    printf("5. COK YUKSEK: %.4f\n", mem.cok_yuksek);
    printf("---------------------------------------------------\n");
}

void FuzzyLogic::printResults(const SensorReadings& inputs, const SystemFuzzyState& state) {
    printSingleResult("SICAKLIK", inputs.temperature, state.temp_mu);
    printSingleResult("NEM", inputs.humidity, state.hum_mu);
    printSingleResult("ISIK", inputs.lightLevel, state.light_mu);
    printSingleResult("TOPRAK NEMI", inputs.soilMoisture, state.soil_mu);
}
