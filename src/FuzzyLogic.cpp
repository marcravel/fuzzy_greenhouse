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
String FuzzyLogic::getBestLabel(float cok_dusuk, float dusuk, float orta, float yuksek, float cok_yuksek) {
    float max = cok_dusuk;
    String label = "cok_dusuk";
    if (dusuk > max) {
        max = dusuk;
        label = "dusuk";
    }
    if (orta > max) {
        max = orta;
        label = "orta";
    }
    if (yuksek > max) {
        max = yuksek;
        label = "yuksek";
    }
    if (cok_yuksek > max) {
        max = cok_yuksek;
        label = "cok_yuksek";
    }
    return label;
}

FuzzyMembership FuzzyLogic::fuzzifyTemperature(float temp) {
    FuzzyMembership mu;
    mu.cok_dusuk  = trapmf_dusen(temp, -10.0f, -10.0f, 0.0f, 10.0f);
    mu.dusuk      = trimf(temp, 0.0f, 7.5f, 15.0f);
    mu.orta       = trimf(temp, 14.0f, 20.0f, 26.0f);
    mu.yuksek     = trimf(temp, 20.0f, 30.0f, 40.0f);
    mu.cok_yuksek = trapmf_yukselen(temp, 30.0f, 40.0f, 50.0f, 50.0f);
    mu.sozel_ifade = getBestLabel(mu.cok_dusuk, mu.dusuk, mu.orta, mu.yuksek, mu.cok_yuksek);
    return mu;
}

FuzzyMembership FuzzyLogic::fuzzifyHumidity(float hum) {
    FuzzyMembership mu;
    mu.cok_dusuk  = trapmf_dusen(hum, 0.0f, 0.0f, 20.0f, 40.0f);
    mu.dusuk      = trimf(hum, 20.0f, 39.5f, 59.0f);
    mu.orta       = trimf(hum, 50.0f, 60.0f, 70.0f);
    mu.yuksek     = trimf(hum, 60.0f, 75.0f, 90.0f);
    mu.cok_yuksek = trapmf_yukselen(hum, 80.0f, 90.0f, 100.0f, 100.0f);
    mu.sozel_ifade = getBestLabel(mu.cok_dusuk, mu.dusuk, mu.orta, mu.yuksek, mu.cok_yuksek);
    return mu;
}

FuzzyMembership FuzzyLogic::fuzzifyLight(float light) {
    FuzzyMembership mu;
    mu.cok_dusuk  = trapmf_dusen(light, 0.0f, 0.0f, 2500.0f, 5000.0f);
    mu.dusuk      = trimf(light, 4000.0f, 6500.0f, 9000.0f);
    mu.orta       = trimf(light, 8000.0f, 10000.0f, 12000.0f);
    mu.yuksek     = trimf(light, 11000.0f, 13500.0f, 16000.0f);
    mu.cok_yuksek = trapmf_yukselen(light, 15000.0f, 17500.0f, 20000.0f, 20000.0f);
    mu.sozel_ifade = getBestLabel(mu.cok_dusuk, mu.dusuk, mu.orta, mu.yuksek, mu.cok_yuksek);
    return mu;
}

FuzzyMembership FuzzyLogic::fuzzifySoilMoisture(float soil) {
    FuzzyMembership mu;
    mu.cok_dusuk  = trapmf_dusen(soil, 0.0f, 0.0f, 20.0f, 40.0f);
    mu.dusuk      = trimf(soil, 30.0f, 49.5f, 69.0f);
    mu.orta       = trimf(soil, 60.0f, 69.5f, 79.0f);
    mu.yuksek     = trimf(soil, 70.0f, 80.0f, 90.0f);
    mu.cok_yuksek = trapmf_yukselen(soil, 80.0f, 90.0f, 100.0f, 100.0f);
    mu.sozel_ifade = getBestLabel(mu.cok_dusuk, mu.dusuk, mu.orta, mu.yuksek, mu.cok_yuksek);
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
    printf("Best term:     %s\n", mem.sozel_ifade);
    printf("---------------------------------------------------\n");
}

void FuzzyLogic::printResults(const SensorReadings& inputs, const SystemFuzzyState& state) {
    printSingleResult("SICAKLIK", inputs.temperature, state.temp_mu);
    printSingleResult("NEM", inputs.humidity, state.hum_mu);
    printSingleResult("ISIK", inputs.lightLevel, state.light_mu);
    printSingleResult("TOPRAK NEMI", inputs.soilMoisture, state.soil_mu);
}

SicaklikEtiket FuzzyLogic::string_to_enum_sicaklik(String etiket)
{
    if (etiket == "cok_dusuk")    return SICAKLIK_LABEL_COK_DUSUK;
    if (etiket == "dusuk")        return SICAKLIK_LABEL_DUSUK;
    if (etiket == "orta")         return SICAKLIK_LABEL_ORTA;
    if (etiket == "yuksek")       return SICAKLIK_LABEL_YUKSEK;
    if (etiket == "cok_yuksek")   return SICAKLIK_LABEL_COK_YUKSEK;
    
    return (SicaklikEtiket)-1; // Cast to enum type to avoid warnings
}

NemEtiket FuzzyLogic::string_to_enum_nem(String etiket)
{
    if (etiket == "cok_dusuk")    return NEM_LABEL_COK_DUSUK;
    if (etiket == "dusuk")        return NEM_LABEL_DUSUK;
    if (etiket == "orta")         return NEM_LABEL_ORTA;
    if (etiket == "yuksek")       return NEM_LABEL_YUKSEK;
    if (etiket == "cok_yuksek")   return NEM_LABEL_COK_YUKSEK;

    return (NemEtiket)-1; // Cast to enum type to avoid warnings
}

IsikEtiket FuzzyLogic::string_to_enum_isik(String etiket)
{
    if (etiket == "cok_dusuk")    return ISIK_LABEL_COK_DUSUK;
    if (etiket == "dusuk")        return ISIK_LABEL_DUSUK;
    if (etiket == "orta")         return ISIK_LABEL_ORTA;
    if (etiket == "yuksek")       return ISIK_LABEL_YUKSEK;
    if (etiket == "cok_yuksek")   return ISIK_LABEL_COK_YUKSEK;
    return (IsikEtiket)-1;
}

NemToprakEtiket FuzzyLogic::string_to_enum_toprak(String etiket)
{
    if (etiket == "cok_dusuk")    return NEMTOPRAK_LABEL_COK_DUSUK;
    if (etiket == "dusuk")        return NEMTOPRAK_LABEL_DUSUK;
    if (etiket == "orta")         return NEMTOPRAK_LABEL_ORTA;
    if (etiket == "yuksek")       return NEMTOPRAK_LABEL_YUKSEK;
    if (etiket == "cok_yuksek")   return NEMTOPRAK_LABEL_COK_YUKSEK;
    return (NemToprakEtiket)-1;
}

SystemDecisions FuzzyLogic::evaluateRules(const SystemFuzzyState& state) {
    SystemDecisions decisions;
    
    // --- Parse Inputs ---
    SicaklikEtiket temp = string_to_enum_sicaklik(state.temp_mu.sozel_ifade);
    NemEtiket hum = string_to_enum_nem(state.hum_mu.sozel_ifade);
    IsikEtiket light = string_to_enum_isik(state.light_mu.sozel_ifade);
    NemToprakEtiket soil = string_to_enum_toprak(state.soil_mu.sozel_ifade);

    // --- RULE 1: Heating (Cikis) ---
    // Inputs: Temp, Hum
    if (temp != -1 && hum != -1) {
        switch (temp) {
            case SICAKLIK_LABEL_COK_DUSUK:
                if (hum == NEM_LABEL_COK_DUSUK || hum == NEM_LABEL_DUSUK) decisions.heating = "cok_yuksek";
                else if (hum == NEM_LABEL_ORTA || hum == NEM_LABEL_YUKSEK || hum == NEM_LABEL_COK_YUKSEK) decisions.heating = "yuksek";
                break;
            case SICAKLIK_LABEL_DUSUK:
                if (hum == NEM_LABEL_COK_DUSUK || hum == NEM_LABEL_DUSUK || hum == NEM_LABEL_ORTA) decisions.heating = "yuksek";
                else if (hum == NEM_LABEL_YUKSEK || hum == NEM_LABEL_COK_YUKSEK) decisions.heating = "orta";
                break;
            case SICAKLIK_LABEL_ORTA:
                if (hum == NEM_LABEL_COK_DUSUK || hum == NEM_LABEL_DUSUK || hum == NEM_LABEL_ORTA || hum == NEM_LABEL_YUKSEK) decisions.heating = "orta";
                else if (hum == NEM_LABEL_COK_YUKSEK) decisions.heating = "dusuk";
                break;
            case SICAKLIK_LABEL_YUKSEK:
                decisions.heating = "dusuk"; 
                break;
            case SICAKLIK_LABEL_COK_YUKSEK:
                decisions.heating = "cok_dusuk";
                break;
        }
    }

    // --- RULE 2: Cooling ---
    // Inputs: Temp, Hum
    if (temp != -1 && hum != -1) {
        switch (temp) {
            case SICAKLIK_LABEL_COK_DUSUK:
                decisions.cooling = "cok_dusuk";
                break;
            case SICAKLIK_LABEL_DUSUK:
                decisions.cooling = "dusuk";
                break;
            case SICAKLIK_LABEL_ORTA:
                if (hum == NEM_LABEL_COK_DUSUK) decisions.cooling = "dusuk";
                else decisions.cooling = "orta";
                break;
            case SICAKLIK_LABEL_YUKSEK:
                if (hum == NEM_LABEL_COK_DUSUK || hum == NEM_LABEL_DUSUK) decisions.cooling = "orta";
                else decisions.cooling = "yuksek";
                break;
            case SICAKLIK_LABEL_COK_YUKSEK:
                if (hum == NEM_LABEL_YUKSEK || hum == NEM_LABEL_COK_YUKSEK) decisions.cooling = "cok_yuksek";
                else decisions.cooling = "yuksek";
                break;
        }
    }
    
    // --- RULE 3: Shadow (Golgeleme) ---
    // Inputs: Temp, Light
    if (temp != -1 && light != -1) {
        switch (temp) {
             case SICAKLIK_LABEL_COK_DUSUK:
                if (light == ISIK_LABEL_COK_DUSUK || light == ISIK_LABEL_DUSUK) decisions.shadow = "cok_dusuk";
                else if (light == ISIK_LABEL_ORTA) decisions.shadow = "dusuk";
                else decisions.shadow = "orta";
                break;
             case SICAKLIK_LABEL_DUSUK:
                if (light == ISIK_LABEL_COK_DUSUK) decisions.shadow = "cok_dusuk";
                else if (light == ISIK_LABEL_DUSUK || light == ISIK_LABEL_ORTA) decisions.shadow = "dusuk";
                else if (light == ISIK_LABEL_YUKSEK) decisions.shadow = "orta";
                else decisions.shadow = "yuksek";
                break;
             case SICAKLIK_LABEL_ORTA:
                if (light == ISIK_LABEL_COK_DUSUK) decisions.shadow = "cok_dusuk";
                else if (light == ISIK_LABEL_DUSUK) decisions.shadow = "dusuk";
                else if (light == ISIK_LABEL_ORTA || light == ISIK_LABEL_YUKSEK) decisions.shadow = "orta";
                else decisions.shadow = "yuksek";
                break;
             case SICAKLIK_LABEL_YUKSEK:
                if (light == ISIK_LABEL_COK_DUSUK || light == ISIK_LABEL_DUSUK) decisions.shadow = "dusuk";
                else if (light == ISIK_LABEL_ORTA) decisions.shadow = "orta";
                else decisions.shadow = "yuksek";
                break;
             case SICAKLIK_LABEL_COK_YUKSEK:
                if (light == ISIK_LABEL_COK_DUSUK) decisions.shadow = "dusuk";
                else if (light == ISIK_LABEL_DUSUK || light == ISIK_LABEL_ORTA) decisions.shadow = "orta";
                else if (light == ISIK_LABEL_YUKSEK) decisions.shadow = "yuksek";
                else decisions.shadow = "cok_yuksek";
                break;
        }
    }

    // --- RULE 4: Water (Sulama) ---
    // Inputs: Hum, Soil
    if (hum != -1 && soil != -1) {
        switch (hum) {
            case NEM_LABEL_COK_DUSUK:
                if (soil == NEMTOPRAK_LABEL_COK_DUSUK || soil == NEMTOPRAK_LABEL_DUSUK) decisions.water = "cok_yuksek";
                else if (soil == NEMTOPRAK_LABEL_ORTA || soil == NEMTOPRAK_LABEL_YUKSEK) decisions.water = "yuksek";
                else decisions.water = "orta";
                break;
            case NEM_LABEL_DUSUK:
                if (soil == NEMTOPRAK_LABEL_COK_DUSUK || soil == NEMTOPRAK_LABEL_DUSUK || soil == NEMTOPRAK_LABEL_ORTA) decisions.water = "yuksek";
                else decisions.water = "orta";
                break;
            case NEM_LABEL_ORTA:
                if (soil == NEMTOPRAK_LABEL_COK_DUSUK) decisions.water = "yuksek";
                else if (soil == NEMTOPRAK_LABEL_COK_YUKSEK) decisions.water = "dusuk";
                else decisions.water = "orta";
                break;
            case NEM_LABEL_YUKSEK:
                if (soil == NEMTOPRAK_LABEL_COK_DUSUK || soil == NEMTOPRAK_LABEL_DUSUK) decisions.water = "orta";
                else decisions.water = "dusuk";
                break;
            case NEM_LABEL_COK_YUKSEK:
                if (soil == NEMTOPRAK_LABEL_COK_DUSUK || soil == NEMTOPRAK_LABEL_DUSUK || soil == NEMTOPRAK_LABEL_ORTA) decisions.water = "dusuk";
                else decisions.water = "cok_dusuk";
                break;
        }
    }
    
    // --- RULE 5: Lighting (Aydinlatma) ---
    // Inputs: Light
    if (light != -1) {
        switch (light) {
            case ISIK_LABEL_COK_DUSUK: decisions.lighting = "cok_yuksek"; break;
            case ISIK_LABEL_DUSUK:     decisions.lighting = "yuksek"; break;
            case ISIK_LABEL_ORTA:      decisions.lighting = "orta"; break;
            case ISIK_LABEL_YUKSEK:    decisions.lighting = "dusuk"; break;
            case ISIK_LABEL_COK_YUKSEK:decisions.lighting = "cok_dusuk"; break;
        }
    }
    
    return decisions;
}