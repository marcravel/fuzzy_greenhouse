#include "FuzzyLogic.h"
#include <math.h>
//Alanlar
float GolgelemeCokDusukAlan=0;
float GolgelemeDusukAlan=0;
float GolgelemeOrtaAlan=0;
float GolgelemeYuksekAlan=0;
float GolgelemeCokYuksekAlan=0;

float SogutmaCokDusukAlan=0;
float SogutmaDusukAlan=0;
float SogutmaOrtaAlan=0;
float SogutmaYuksekAlan=0;
float SogutmaCokYuksekAlan=0;

float SicaklikCokDusukAlan=0;
float SicaklikDusukAlan=0;
float SicaklikOrtaAlan=0;
float SicaklikYuksekAlan=0;
float SicaklikCokYuksekAlan=0;

float SulamaCokDusukAlan=0;
float SulamaDusukAlan=0;
float SulamaOrtaAlan=0;
float SulamaYuksekAlan=0;
float SulamaCokYuksekAlan=0;

float IsiklandirmaAlan = 0.0f;
//-----------
//Have nemi ve toprak nemi: Sulama
float humidity_soil_prim_sec=0;
float humidity_soil_prim_prim=0;
float humidity_soil_sec_sec=0;
float humidity_soil_sec_prim=0;
//sicaklik ve nem: Isitma - Sogutma
float temp_humidity_prim_sec;
float temp_humidity_prim_prim;
float temp_humidity_sec_sec;
float temp_humidity_sec_prim;
//Sicaklik ve isik siddeti: Golgeleme
float temp_light_prim_sec;
float temp_light_prim_prim;
float temp_light_sec_sec;
float temp_light_sec_prim;

char cikis[50]="";//sıcaklık cıkısı x
char cikis1[50]="";
char cikis2[50]="";
char cikis3[50]="";

char Cooling[50]=""; // soğutma cıkısı x
char Cooling1[50]="";
char Cooling2[50]="";
char Cooling3[50]="";

char shadow[50]=""; // gölgelenem cıkısı
char shadow1[50]="";
char shadow2[50]="";
char shadow3[50]="";

char water[50];
char water1[50];
char water2[50];
char water3[50];

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

float FuzzyLogic::min(float a, float b) {
    return (a < b) ? a : b;
}

float FuzzyLogic::max(float a, float b) {
    return (a > b) ? a : b;
}

void FuzzyLogic::get_primary_secondary(FuzzyResult *result, float cd, float d, float o, float y, float cy) {
    float uyelik_degerleri[5];
    uyelik_degerleri[0] = cd;
    uyelik_degerleri[1] = d;
    uyelik_degerleri[2] = o;
    uyelik_degerleri[3] = y;
    uyelik_degerleri[4] = cy;

    float primary = uyelik_degerleri[0];
    int index_primary = 0;
    float secondary = uyelik_degerleri[0];
    int index_secondary = 0;
    for (int i = 1; i < 5; i++) {
        if (uyelik_degerleri[i] > primary) {
            primary = uyelik_degerleri[i];
            index_primary = i;
        }
    }
    result->primary_mu = primary;
    if (index_primary == 0) {
        snprintf(result->primary_label, sizeof(result->primary_label), "%s", "cok_dusuk");
    }
    else if (index_primary == 1) {
        snprintf(result->primary_label, sizeof(result->primary_label), "%s", "dusuk");
    }
    else if (index_primary == 2) {
        snprintf(result->primary_label, sizeof(result->primary_label), "%s", "orta");
    }
    else if (index_primary == 3) {
        snprintf(result->primary_label, sizeof(result->primary_label), "%s", "yuksek");
    }
    else if (index_primary == 4) {
        snprintf(result->primary_label, sizeof(result->primary_label), "%s", "cok_yuksek");
    }

    for (int i = 0; i < 5; i++) {
        if (i != index_primary && uyelik_degerleri[i] > 0) {
            secondary = uyelik_degerleri[i];
            index_secondary = i;
            break;
        }
    }
    result->secondary_mu = secondary;
    if (index_secondary == 0) {
        snprintf(result->secondary_label, sizeof(result->secondary_label), "%s", "cok_dusuk");
    }
    else if (index_secondary == 1) {
        snprintf(result->secondary_label, sizeof(result->secondary_label), "%s", "dusuk");
    }
    else if (index_secondary == 2) {
        snprintf(result->secondary_label, sizeof(result->secondary_label), "%s", "orta");
    }
    else if (index_secondary == 3) {
        snprintf(result->secondary_label, sizeof(result->secondary_label), "%s", "yuksek");
    }
    else if (index_secondary == 4) {
        snprintf(result->secondary_label, sizeof(result->secondary_label), "%s", "cok_yuksek");
    }
}

FuzzyResults FuzzyLogic::calculateFuzzyResults(const SystemFuzzyState& state) {
    FuzzyResults results;
    get_primary_secondary(&results.temp_result, state.temp_mu.cok_dusuk, state.temp_mu.dusuk, state.temp_mu.orta, state.temp_mu.yuksek, state.temp_mu.cok_yuksek);
    get_primary_secondary(&results.hum_result, state.hum_mu.cok_dusuk, state.hum_mu.dusuk, state.hum_mu.orta, state.hum_mu.yuksek, state.hum_mu.cok_yuksek);
    get_primary_secondary(&results.light_result, state.light_mu.cok_dusuk, state.light_mu.dusuk, state.light_mu.orta, state.light_mu.yuksek, state.light_mu.cok_yuksek);
    get_primary_secondary(&results.soil_result, state.soil_mu.cok_dusuk, state.soil_mu.dusuk, state.soil_mu.orta, state.soil_mu.yuksek, state.soil_mu.cok_yuksek);
    return results;
}

void FuzzyLogic::evaluate_rules_temp_light_shadow(const char *SicaklikSozel,const char *IsikSozel) {
    SicaklikEtiket current_label = string_to_enum_sicaklik(SicaklikSozel);
    IsikEtiket current_label1= string_to_enum_isik(IsikSozel);
    switch (current_label)
    {
        case SICAKLIK_LABEL_COK_DUSUK:
        switch (current_label1)
        {
        case ISIK_LABEL_COK_DUSUK:
           strcpy(shadow,"cok_dusuk");
            break;
        case ISIK_LABEL_DUSUK:
           strcpy(shadow,"cok_dusuk");
            break;
        case ISIK_LABEL_ORTA:
           strcpy(shadow,"dusuk");
            break;
            case ISIK_LABEL_YUKSEK:
           strcpy(shadow,"orta");
            break;
            case ISIK_LABEL_COK_YUKSEK:
           strcpy(shadow,"orta");
            break;
        }
            break;
        case SICAKLIK_LABEL_DUSUK:
        switch (current_label1)
        {
        case ISIK_LABEL_COK_DUSUK:
           strcpy(shadow,"cok_dusuk");
            break;
        case ISIK_LABEL_DUSUK:
           strcpy(shadow,"dusuk");
            break;
        case ISIK_LABEL_ORTA:
           strcpy(shadow,"dusuk");
            break;
            case ISIK_LABEL_YUKSEK:
           strcpy(shadow,"orta");
            break;
            case ISIK_LABEL_COK_YUKSEK:
           strcpy(shadow,"yuksek");
            break;
        }
        break;
        case SICAKLIK_LABEL_ORTA:
          switch (current_label1)
        {
        case ISIK_LABEL_COK_DUSUK:
           strcpy(shadow,"cok_dusuk");
            break;
        case ISIK_LABEL_DUSUK:
           strcpy(shadow,"dusuk");
            break;
        case ISIK_LABEL_ORTA:
           strcpy(shadow,"orta");
            break;
            case ISIK_LABEL_YUKSEK:
           strcpy(shadow,"orta");
            break;
            case ISIK_LABEL_COK_YUKSEK:
           strcpy(shadow,"yuksek");
            break;
        }
        break;
         case SICAKLIK_LABEL_YUKSEK:
          switch (current_label1)
        {
        case ISIK_LABEL_COK_DUSUK:
           strcpy(shadow,"dusuk");
            break;
        case ISIK_LABEL_DUSUK:
           strcpy(shadow,"dusuk");
            break;
        case ISIK_LABEL_ORTA:
           strcpy(shadow,"orta");
            break;
            case ISIK_LABEL_YUKSEK:
           strcpy(shadow,"yuksek");
            break;
            case ISIK_LABEL_COK_YUKSEK:
           strcpy(shadow,"yuksek");
            break;
        }
        break;
         case SICAKLIK_LABEL_COK_YUKSEK:
          switch (current_label1)
        {
        case ISIK_LABEL_COK_DUSUK:
           strcpy(shadow,"dusuk");
            break;
        case ISIK_LABEL_DUSUK:
           strcpy(shadow,"orta");
            break;
        case ISIK_LABEL_ORTA:
           strcpy(shadow,"orta");
            break;
            case ISIK_LABEL_YUKSEK:
           strcpy(shadow,"yuksek");
            break;
            case ISIK_LABEL_COK_YUKSEK:
           strcpy(shadow,"cok_yuksek");
            break;
        }
        break;
        default:
        printf("Sicaklik etiketi: %s\n",SicaklikSozel);
        printf("HATA: Tanimsiz Sicaklik Etiketi.\n");
        break;
    }
}

void FuzzyLogic::evaluate_rules_temp_hum_cooling(const char *SicaklikSozel,const char *HavaNemiSozel) {
    SicaklikEtiket current_label = string_to_enum_sicaklik(SicaklikSozel);
    NemEtiket current_label1= string_to_enum_nem(HavaNemiSozel);
    switch (current_label)
    {
        case SICAKLIK_LABEL_COK_DUSUK:
        switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(Cooling,"cok_dusuk");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(Cooling,"cok_dusuk");
            break;
        case NEM_LABEL_ORTA:
           strcpy(Cooling,"cok_dusuk");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(Cooling,"cok_dusuk");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(Cooling,"cok_dusuk");
            break;
        }
            break;
        case SICAKLIK_LABEL_DUSUK:
        switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(Cooling,"dusuk");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(Cooling,"dusuk");
            break;
        case NEM_LABEL_ORTA:
           strcpy(Cooling,"dusuk");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(Cooling,"dusuk");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(Cooling,"dusuk");
            break;
        }
        break;
        case SICAKLIK_LABEL_ORTA:
          switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(Cooling,"dusuk");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(Cooling,"orta");
            break;
        case NEM_LABEL_ORTA:
           strcpy(Cooling,"orta");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(Cooling,"orta");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(Cooling,"orta");
            break;
        }
        break;
         case SICAKLIK_LABEL_YUKSEK:
          switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(Cooling,"orta");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(Cooling,"orta");
            break;
        case NEM_LABEL_ORTA:
           strcpy(Cooling,"yuksek");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(Cooling,"yuksek");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(Cooling,"yuksek");
            break;
        }
        break;
         case SICAKLIK_LABEL_COK_YUKSEK:
          switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(Cooling,"yuksek");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(Cooling,"yuksek");
            break;
        case NEM_LABEL_ORTA:
           strcpy(Cooling,"yuksek");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(Cooling,"cok_yuksek");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(Cooling,"cok_yuksek");
            break;
        }
        break;
        default:
            printf("Sicaklik etiketi: %s\n",SicaklikSozel);
            printf("HATA: Tanimsiz Sicaklik Etiketi.\n");
            break;
    }
}

void FuzzyLogic::evaluate_rules_temp_hum_heating(const char *SicaklikSozel,const char *HavaNemiSozel) {
    SicaklikEtiket current_label = string_to_enum_sicaklik(SicaklikSozel);
    NemEtiket current_label1= string_to_enum_nem(HavaNemiSozel);
    switch (current_label)
    {
        case SICAKLIK_LABEL_COK_DUSUK:
        switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(cikis,"cok_yuksek");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(cikis,"cok_yuksek");
            break;
        case NEM_LABEL_ORTA:
           strcpy(cikis,"yuksek");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(cikis,"yuksek");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(cikis,"yuksek");
            break;
        }
            break;
        case SICAKLIK_LABEL_DUSUK:
        switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(cikis,"yuksek");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(cikis,"yuksek");
            break;
        case NEM_LABEL_ORTA:
           strcpy(cikis,"yuksek");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(cikis,"orta");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(cikis,"orta");
            break;
        }
        break;
        case SICAKLIK_LABEL_ORTA:
          switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(cikis,"orta");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(cikis,"orta");
            break;
        case NEM_LABEL_ORTA:
           strcpy(cikis,"orta");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(cikis,"orta");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(cikis,"dusuk");
            break;
        }
        break;
         case SICAKLIK_LABEL_YUKSEK:
          switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(cikis,"dusuk");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(cikis,"dusuk");
            break;
        case NEM_LABEL_ORTA:
           strcpy(cikis,"dusuk");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(cikis,"dusuk");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(cikis,"dusuk");
            break;
        }
        break;
         case SICAKLIK_LABEL_COK_YUKSEK:
          switch (current_label1)
        {
        case NEM_LABEL_COK_DUSUK:
           strcpy(cikis,"cok_dusuk");
            break;
        case NEM_LABEL_DUSUK:
           strcpy(cikis,"cok_dusuk");
            break;
        case NEM_LABEL_ORTA:
           strcpy(cikis,"cok_dusuk");
            break;
            case NEM_LABEL_YUKSEK:
           strcpy(cikis,"cok_dusuk");
            break;
            case NEM_LABEL_COK_YUKSEK:
           strcpy(cikis,"cok_dusuk");
            break;
        }
        break;
        default:
            printf("Sicaklik etiketi: %s\n",SicaklikSozel);
            printf("HATA: Tanimsiz Sicaklik Etiketi.\n");
            break;
    }
}

void FuzzyLogic::evaluate_rules_hum_soil(char *humidity_label, char *soil_label) {
    NemEtiket curr_label_humidity = string_to_enum_nem(humidity_label);
    NemToprakEtiket curr_label_soil= string_to_enum_toprak(soil_label);
    switch (curr_label_humidity)
    {
        case NEM_LABEL_COK_DUSUK:
        switch (curr_label_soil)
        {
        case NEMTOPRAK_LABEL_COK_DUSUK:
           strcpy(water,"cok_yuksek");
            break;
        case NEMTOPRAK_LABEL_DUSUK:
           strcpy(water,"cok_yuksek");
            break;
        case NEMTOPRAK_LABEL_ORTA:
           strcpy(water,"yuksek");
            break;
            case NEMTOPRAK_LABEL_YUKSEK:
           strcpy(water,"yuksek");
            break;
            case NEMTOPRAK_LABEL_COK_YUKSEK:
           strcpy(water,"orta");
            break;
        }
            break;
        case NEM_LABEL_DUSUK:
        switch (curr_label_soil)
        {
        case NEMTOPRAK_LABEL_COK_DUSUK:
           strcpy(water,"yuksek");
            break;
        case NEMTOPRAK_LABEL_DUSUK:
           strcpy(water,"yuksek");
            break;
        case NEMTOPRAK_LABEL_ORTA:
           strcpy(water,"yuksek");
            break;
            case NEMTOPRAK_LABEL_YUKSEK:
           strcpy(water,"orta");
            break;
            case NEMTOPRAK_LABEL_COK_YUKSEK:
           strcpy(water,"orta");
            break;
        }
        break;
        case NEM_LABEL_ORTA:
          switch (curr_label_soil)
        {
        case NEMTOPRAK_LABEL_COK_DUSUK:
           strcpy(water,"yuksek");
            break;
        case NEMTOPRAK_LABEL_DUSUK:
           strcpy(water,"orta");
            break;
        case NEMTOPRAK_LABEL_ORTA:
           strcpy(water,"orta");
            break;
            case NEMTOPRAK_LABEL_YUKSEK:
           strcpy(water,"orta");
            break;
            case NEMTOPRAK_LABEL_COK_YUKSEK:
           strcpy(water,"dusuk");
            break;
        }
        break;
         case NEM_LABEL_YUKSEK:
          switch (curr_label_soil)
        {
        case NEMTOPRAK_LABEL_COK_DUSUK:
           strcpy(water,"orta");
            break;
        case NEMTOPRAK_LABEL_DUSUK:
           strcpy(water,"orta");
            break;
        case NEMTOPRAK_LABEL_ORTA:
           strcpy(water,"dusuk");
            break;
            case NEMTOPRAK_LABEL_YUKSEK:
           strcpy(water,"dusuk");
            break;
            case NEMTOPRAK_LABEL_COK_YUKSEK:
           strcpy(water,"dusuk");
            break;
        }
        break;
         case NEM_LABEL_COK_YUKSEK:
          switch (curr_label_soil)
        {
        case NEMTOPRAK_LABEL_COK_DUSUK:
           strcpy(water,"dusuk");
            break;
        case NEMTOPRAK_LABEL_DUSUK:
           strcpy(water,"dusuk");
            break;
        case NEMTOPRAK_LABEL_ORTA:
           strcpy(water,"dusuk");
            break;
            case NEMTOPRAK_LABEL_YUKSEK:
           strcpy(water,"cok_dusuk");
            break;
            case NEMTOPRAK_LABEL_COK_YUKSEK:
           strcpy(water,"cok_dusuk");
            break;
        }
        break;
        default:
            printf("Nem etiketi: %s\n", humidity_label);
            printf("HATA: Tanimsiz nem Etiketi.\n");
            break;
    }
}

void FuzzyLogic::evaluate_rules_light(char *light_label, SystemDecisions *system_decisions) {
    IsikEtiket current_label = string_to_enum_isik(light_label);
    switch (current_label)
    {
    case  ISIK_LABEL_COK_DUSUK:
        strcpy(system_decisions->output_label_light,"cok_yuksek");
        break;
    case  ISIK_LABEL_DUSUK:
        strcpy(system_decisions->output_label_light,"yuksek");
        break;
    case  ISIK_LABEL_ORTA:
        strcpy(system_decisions->output_label_light,"orta");
        break;
    case  ISIK_LABEL_YUKSEK:
        strcpy(system_decisions->output_label_light,"dusuk");
        break;
    case  ISIK_LABEL_COK_YUKSEK:
        strcpy(system_decisions->output_label_light,"cok_dusuk");
        break;
    default:
        printf("Isik etiketi: %s\n",light_label);
        printf("HATA: Tanimsiz Isik Etiketi.\n");
    break;
    }
}

float FuzzyLogic::GolgelemeDurulastirmaCokDusuk (float x){
     if (x==0)
    {
        return x;
    }
    float y=0;
    y=1-x;
    x=x*25;
    y=12.5-y*y*12.5;
    GolgelemeCokDusukAlan=GolgelemeCokDusukAlan+x+y;
    x=(12.5*x+y*(25+(25/3)));
    return x;
}
float FuzzyLogic::GolgelemeDurulastirmaDusuk (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=75-x*x*75;
    GolgelemeDusukAlan=GolgelemeDusukAlan+x;
    x=x*75;
    return x;
}
float FuzzyLogic::GolgelemeDurulastirmaOrta (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=50-x*x*50;
    GolgelemeOrtaAlan=GolgelemeOrtaAlan+x;
    x=x*150;
    return x;
}
float FuzzyLogic::GolgelemeDurulastirmaYuksek (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=45-x*x*45;
    GolgelemeYuksekAlan=GolgelemeYuksekAlan+x;
    x=x*205;
    return x;
}
float FuzzyLogic::GolgelemeDurulastirmaCokYuksek (float x){
    if (x==0)
    {
        return x;
    }
    float y=0;
    y=1-x;
    y=25-y*y*25;
    x=x*45;
    GolgelemeCokYuksekAlan=GolgelemeCokYuksekAlan+x+y;
    x=(277.5*x+y*(205+(100/3)));
    return x;
}

float FuzzyLogic::SogutmaDurulastirmaCokDusuk (float x){
     if (x==0)
    {
        return x;
    }
    float y=0;
    y=1-x;
    x=x*2.5;
    y=1.25-y*y*1.25;
    SogutmaCokDusukAlan=SogutmaCokDusukAlan+x+y;
    x=(1.25*x+y*(2.5+(2.5/3)));
    return x;
}
float FuzzyLogic::SogutmaDurulastirmaDusuk (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=7.5-x*x*7.5;
    SogutmaDusukAlan=SogutmaDusukAlan+x;
    x=x*7.5;
    return x;
}
float FuzzyLogic::SogutmaDurulastirmaOrta (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=5-x*x*5;
    SogutmaOrtaAlan=SogutmaOrtaAlan+x;
    x=x*15;
    return x;
}
float FuzzyLogic::SogutmaDurulastirmaYuksek (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=4.5-x*x*4.5;
    SogutmaYuksekAlan=SogutmaYuksekAlan+x;
    x=x*20.5;
    return x;
}
float FuzzyLogic::SogutmaDurulastirmaCokYuksek (float x){
    if (x==0)
    {
        return x;
    }
    float y=0;
    y=1-x;
    y=2-y*y*2;
    x=x*4;
    SogutmaCokYuksekAlan=SogutmaCokYuksekAlan+x+y;
    x=(28*x+y*(26+(8/3)));
    return x;   
}

float FuzzyLogic::SicaklikDurulastirmaCokDusuk (float x){
     if (x==0)
    {
        return x;
    }
    float y=0;
    y=1-x;
    x=x*0.5;
    y=0.25-y*y*0.25;
    SicaklikCokDusukAlan=SicaklikCokDusukAlan+x+y;
    x=(0.25*x+y*(0.5+(0.5/3)));
    return x;
}
float FuzzyLogic::SicaklikDurulastirmaDusuk (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=1.5-x*x*1.5;
    SicaklikDusukAlan=SicaklikDusukAlan+x;
    x=x*1.5;
    return x;
}
float FuzzyLogic::SicaklikDurulastirmaOrta (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=1-x*x;
    SicaklikOrtaAlan=SicaklikOrtaAlan+x;
    x=x*3;
    return x;
}
float FuzzyLogic::SicaklikDurulastirmaYuksek (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=0.75-x*x*0.75;
    SicaklikYuksekAlan=SicaklikYuksekAlan+x;
    x=x*4.25;
    return x;
}
float FuzzyLogic::SicaklikDurulastirmaCokYuksek (float x){
    if (x==0)
    {
        return x;
    }
    float y=0;
    y=1-x;
    y=0.375-y*y*0.375;
    x=x*0.75;
    SicaklikCokYuksekAlan=SicaklikCokYuksekAlan+x+y;
    x=(5.625*x+y*5);
    return x;
}

float FuzzyLogic::SulamaDurulastirmaCokDusuk (float x){
     if (x==0)
    {
        return x;
    }
    float y=0;
    y=1-x;
    x=x*2;
    y=1-y*y;
    SulamaCokDusukAlan=SulamaCokDusukAlan+x+y;
    x=(x+y*(2+(2/3)));
    return x;
}
float FuzzyLogic::SulamaDurulastirmaDusuk (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=9.5-x*x*9.5;
    SulamaDusukAlan=SulamaDusukAlan+x;
    x=x*9.5;
    return x;
}
float FuzzyLogic::SulamaDurulastirmaOrta (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=9.5-x*x*9.5;
    SulamaOrtaAlan=SulamaOrtaAlan+x;
    x=x*19.5;
    return x;
}
float FuzzyLogic::SulamaDurulastirmaYuksek (float x){
     if (x==0)
    {
        return x;
    }
    x=1-x;
    x=10-x*x*10;
    SulamaYuksekAlan=SulamaYuksekAlan+x;
    x=x*30;
    return x;
}
float FuzzyLogic::SulamaDurulastirmaCokYuksek (float x){
    if (x==0)
    {
        return x;
    }
    float y=0;
    y=1-x;
    y=5-y*y*5;
    x=x*10;
    SulamaCokYuksekAlan=SulamaCokYuksekAlan+x+y;
    x=(45*x+y*(30+(20/3)));
    return x;
}

float FuzzyLogic::IsiklandirmaDurulastirmaCokDusuk (float x){
     if (x==0)
    {
        return x;
    }
    float y=0.0f;
    y=1.0f-x;
    x=x*500.0f;
    y=250.0f-y*y*250.0f;
    IsiklandirmaAlan=IsiklandirmaAlan+x+y;
    x=(250.0f*x+y*(500.0f+(500.0f/3.0f))); 
    return x;
}
float FuzzyLogic::IsiklandirmaDurulastirmaDusuk (float x){
     if (x==0.0f)
    {
        return x;
    }
    x=1-x;
    x=5000.0f-x*x*5000.0f;
    IsiklandirmaAlan=IsiklandirmaAlan+x;
    x=x*5000.0f;
    return x;
}
float FuzzyLogic::IsiklandirmaDurulastirmaOrta (float x){
     if (x==0.0f)
    {
        return x;
    }
        x=1.0f-x;
    x=2000.0f-x*x*2000.0f;
    IsiklandirmaAlan=IsiklandirmaAlan+x;
    x=x*10000.0f;
    return x;
}
float FuzzyLogic::IsiklandirmaDurulastirmaYuksek (float x){
     if (x==0.0f)
    {
        return x;
    }
        x=1.0f-x;
    x=2500.0f-x*x*2500.0f;
    IsiklandirmaAlan=IsiklandirmaAlan+x;
    x=x*13500.0f;
    return x;
}
float FuzzyLogic::IsiklandirmaDurulastirmaCokYuksek (float x){
    if (x==0.0)
    {
        return x;
    }
    float y=0.0;
    y=1.0-x;
    y=1250.0-y*y*1250.0;
    x=x*4000.0;
    IsiklandirmaAlan=IsiklandirmaAlan+x+y;
    x=(18000.0*x+y*(13500.0+(5000.0/3.0)));
    return x;
}

float FuzzyLogic::shadow_defuzzy(float x,float y,float z,float t){
    float a=0;
    float b=0;
    float c=0;
    float d=0;
    if (strcmp(shadow,shadow1)==0)
    {
       x=max(x,y);
       y=0;
    }
     if (strcmp(shadow,shadow2)==0)
    {
       x=max(x,z);
       z=0;
    }
     if (strcmp(shadow,shadow3)==0)
    {
        x=max(x,t);
        t=0;
    }
     if (strcmp(shadow1,shadow2)==0)
    {
        y=max(y,z);
        z=0;
    }
     if (strcmp(shadow1,shadow3)==0)
    {
       y=max(y,t);
       t=0;
    }
     if (strcmp(shadow2,shadow3)==0)
    {
        z=max(z,t);
        t=0;
    }
    if (strcmp(shadow1,"cok_dusuk")==0 && y!=0)
    {
    d=GolgelemeDurulastirmaCokDusuk(y);
    }
    else if (strcmp(shadow1,"dusuk")==0 && y!=0)
    {
    d=GolgelemeDurulastirmaDusuk(y);
    }
    else if (strcmp(shadow1,"orta")==0 && y!=0)
    {
    d=GolgelemeDurulastirmaOrta(y);
    }
    else if (strcmp(shadow1,"yuksek")==0 && y!=0)
    {
    d=GolgelemeDurulastirmaYuksek(y);
    }
    else if (strcmp(shadow1,"cok_yuksek")==0 && y!=0)
    {
    d=GolgelemeDurulastirmaCokYuksek(y);
    }
    if (strcmp(shadow,"cok_dusuk")==0 && x!=0)
    {
    a=GolgelemeDurulastirmaCokDusuk(x);
    }
    else if (strcmp(shadow,"dusuk")==0 && x!=0)
    {
    a=GolgelemeDurulastirmaDusuk(x);
    }
    else if (strcmp(shadow,"orta")==0 && x!=0)
    {
    a=GolgelemeDurulastirmaOrta(x);
    }
    else if (strcmp(shadow,"yuksek")==0 && x!=0)
    {
    a=GolgelemeDurulastirmaYuksek(x);
    }
    else if (strcmp(shadow,"cok_yuksek")==0 && x!=0)
    {
    a=GolgelemeDurulastirmaCokYuksek(x);
    }
    if (strcmp(shadow2,"cok_dusuk")==0 && z!=0)
    {
    b=GolgelemeDurulastirmaCokDusuk(z);
    }
    else if (strcmp(shadow2,"dusuk")==0 && z!=0)
    {
    b=GolgelemeDurulastirmaDusuk(z);
    }
    else if (strcmp(shadow2,"orta")==0 && z!=0)
    {
    b=GolgelemeDurulastirmaOrta(z);
    }
    else if (strcmp(shadow2,"yuksek")==0 && z!=0)
    {
    b=GolgelemeDurulastirmaYuksek(z);
    }
    else if (strcmp(shadow2,"cok_yuksek")==0 && z!=0)
    {
    b=GolgelemeDurulastirmaCokYuksek(z);
    }
    if (strcmp(shadow3,"cok_dusuk")==0 && t!=0)
    {
    c=GolgelemeDurulastirmaCokDusuk(t);
    }
    else if (strcmp(shadow3,"dusuk")==0 && t!=0)
    {
    c=GolgelemeDurulastirmaDusuk(t);
    }
    else if (strcmp(shadow3,"orta")==0 && t!=0)
    {
    c=GolgelemeDurulastirmaOrta(t);
    }
    else if (strcmp(shadow3,"yuksek")==0 && t!=0)
    {
    c=GolgelemeDurulastirmaYuksek(t);
    }
    else if (strcmp(shadow3,"cok_yuksek")==0 && t!=0)
    {
    c=GolgelemeDurulastirmaCokYuksek(t);
    }
    a=(a+b+c+d)/(GolgelemeCokDusukAlan+GolgelemeDusukAlan+GolgelemeOrtaAlan+GolgelemeYuksekAlan+GolgelemeCokYuksekAlan);
    GolgelemeCokDusukAlan=0;
    GolgelemeDusukAlan=0;
    GolgelemeOrtaAlan=0;
    GolgelemeYuksekAlan=0;
    GolgelemeCokYuksekAlan=0;
    return a;
}

float FuzzyLogic::cooling_defuzzy(float x,float y,float z,float t){
    float a=0;
    float b=0;
    float c=0;
    float d=0;
    if (strcmp(Cooling,Cooling1)==0)
    {
       x=max(x,y);
       y=0;
    }
     if (strcmp(Cooling,Cooling2)==0)
    {
       x=max(x,z);
       z=0;
    }
     if (strcmp(Cooling,Cooling3)==0)
    {
        x=max(x,t);
        t=0;
    }
     if (strcmp(Cooling1,Cooling2)==0)
    {
        y=max(y,z);
        z=0;
    }
     if (strcmp(Cooling1,Cooling3)==0)
    {
       y=max(y,t);
       t=0;
    }
     if (strcmp(Cooling2,Cooling3)==0)
    {
        z=max(z,t);
        t=0;
    }
    if (strcmp(Cooling1,"cok_dusuk")==0 && y!=0)
    {
    d=SogutmaDurulastirmaCokDusuk(y);
    }
    else if (strcmp(Cooling1,"dusuk")==0 && y!=0)
    {
    d=SogutmaDurulastirmaDusuk(y);
    }
    else if (strcmp(Cooling1,"orta")==0 && y!=0)
    {
    d=SogutmaDurulastirmaOrta(y);
    }
    else if (strcmp(Cooling1,"yuksek")==0 && y!=0)
    {
    d=SogutmaDurulastirmaYuksek(y);
    }
    else if (strcmp(Cooling1,"cok_yuksek")==0 && y!=0)
    {
    d=SogutmaDurulastirmaCokYuksek(y);
    }
    if (strcmp(Cooling,"cok_dusuk")==0 && x!=0)
    {
    a=SogutmaDurulastirmaCokDusuk(x);
    }
    else if (strcmp(Cooling,"dusuk")==0 && x!=0)
    {
    a=SogutmaDurulastirmaDusuk(x);
    }
    else if (strcmp(Cooling,"orta")==0 && x!=0)
    {
    a=SogutmaDurulastirmaOrta(x);
    }
    else if (strcmp(Cooling,"yuksek")==0 && x!=0)
    {
    a=SogutmaDurulastirmaYuksek(x);
    }
    else if (strcmp(Cooling,"cok_yuksek")==0 && x!=0)
    {
    a=SogutmaDurulastirmaCokYuksek(x);
    }
    if (strcmp(Cooling2,"cok_dusuk")==0 && z!=0)
    {
    b=SogutmaDurulastirmaCokDusuk(z);
    }
    else if (strcmp(Cooling2,"dusuk")==0 && z!=0)
    {
    b=SogutmaDurulastirmaDusuk(z);
    }
    else if (strcmp(Cooling2,"orta")==0 && z!=0)
    {
    b=SogutmaDurulastirmaOrta(z);
    }
    else if (strcmp(Cooling2,"yuksek")==0 && z!=0)
    {
    b=SogutmaDurulastirmaYuksek(z);
    }
    else if (strcmp(Cooling2,"cok_yuksek")==0 && z!=0)
    {
    b=SogutmaDurulastirmaCokYuksek(z);
    }
    if (strcmp(Cooling3,"cok_dusuk")==0 && t!=0)
    {
    c=SogutmaDurulastirmaCokDusuk(t);
    }
    else if (strcmp(Cooling3,"dusuk")==0 && t!=0)
    {
    c=SogutmaDurulastirmaDusuk(t);
    }
    else if (strcmp(Cooling3,"orta")==0 && t!=0)
    {
    c=SogutmaDurulastirmaOrta(t);
    }
    else if (strcmp(Cooling3,"yuksek")==0 && t!=0)
    {
    c=SogutmaDurulastirmaYuksek(t);
    }
    else if (strcmp(Cooling3,"cok_yuksek")==0 && t!=0)
    {
    c=SogutmaDurulastirmaCokYuksek(t);
    }
    a=(a+b+c+d)/(SogutmaCokDusukAlan+SogutmaDusukAlan+SogutmaOrtaAlan+SogutmaYuksekAlan+SogutmaCokYuksekAlan);
    SogutmaCokDusukAlan=0;
    SogutmaDusukAlan=0;
    SogutmaOrtaAlan=0;
    SogutmaYuksekAlan=0;
    SogutmaCokYuksekAlan=0;
    return a;
}

float FuzzyLogic::heat_defuzzy(float x,float y,float z,float t){
    float a=0;
    float b=0;
    float c=0;
    float d=0;
    if (strcmp(cikis,cikis1)==0)
    {
       x=max(x,y);
       y=0;
    }
     if (strcmp(cikis,cikis2)==0)
    {
       x=max(x,z);
       z=0;
    }
     if (strcmp(cikis,cikis3)==0)
    {
        x=max(x,t);
        t=0;
    }
     if (strcmp(cikis1,cikis2)==0)
    {
        y=max(y,z);
        z=0;
    }
     if (strcmp(cikis1,cikis3)==0)
    {
       y=max(y,t);
       t=0;
    }
     if (strcmp(cikis2,cikis3)==0)
    {
        z=max(z,t);
        t=0;
    }
    if (strcmp(cikis1,"cok_dusuk")==0 && y!=0)
    {
    d=SicaklikDurulastirmaCokDusuk(y);
    }
    else if (strcmp(cikis1,"dusuk")==0 && y!=0)
    {
    d=SicaklikDurulastirmaDusuk(y);
    }
    else if (strcmp(cikis1,"orta")==0 && y!=0)
    {
    d=SicaklikDurulastirmaOrta(y);
    }
    else if (strcmp(cikis1,"yuksek")==0 && y!=0)
    {
    d=SicaklikDurulastirmaYuksek(y);
    }
    else if (strcmp(cikis1,"cok_yuksek")==0 && y!=0)
    {
    d=SicaklikDurulastirmaCokYuksek(y);
    }
    if (strcmp(cikis,"cok_dusuk")==0 && x!=0)
    {
    a=SicaklikDurulastirmaCokDusuk(x);
    }
    else if (strcmp(cikis,"dusuk")==0 && x!=0)
    {
    a=SicaklikDurulastirmaDusuk(x);
    }
    else if (strcmp(cikis,"orta")==0 && x!=0)
    {
    a=SicaklikDurulastirmaOrta(x);
    }
    else if (strcmp(cikis,"yuksek")==0 && x!=0)
    {
    a=SicaklikDurulastirmaYuksek(x);
    }
    else if (strcmp(cikis,"cok_yuksek")==0 && x!=0)
    {
    a=SicaklikDurulastirmaCokYuksek(x);
    }
    if (strcmp(cikis2,"cok_dusuk")==0 && z!=0)
    {
    b=SicaklikDurulastirmaCokDusuk(z);
    }
    else if (strcmp(cikis2,"dusuk")==0 && z!=0)
    {
    b=SicaklikDurulastirmaDusuk(z);
    }
    else if (strcmp(cikis2,"orta")==0 && z!=0)
    {
    b=SicaklikDurulastirmaOrta(z);
    }
    else if (strcmp(cikis2,"yuksek")==0 && z!=0)
    {
    b=SicaklikDurulastirmaYuksek(z);
    }
    else if (strcmp(cikis2,"cok_yuksek")==0 && z!=0)
    {
    b=SicaklikDurulastirmaCokYuksek(z);
    }
    if (strcmp(cikis3,"cok_dusuk")==0 && t!=0)
    {
    c=SicaklikDurulastirmaCokDusuk(t);
    }
    else if (strcmp(cikis3,"dusuk")==0 && t!=0)
    {
    c=SicaklikDurulastirmaDusuk(t);
    }
    else if (strcmp(cikis3,"orta")==0 && t!=0)
    {
    c=SicaklikDurulastirmaOrta(t);
    }
    else if (strcmp(cikis3,"yuksek")==0 && t!=0)
    {
    c=SicaklikDurulastirmaYuksek(t);
    }
    else if (strcmp(cikis3,"cok_yuksek")==0 && t!=0)
    {
    c=SicaklikDurulastirmaCokYuksek(t);
    }
    a=(a+b+c+d)/(SicaklikCokDusukAlan+SicaklikDusukAlan+SicaklikOrtaAlan+SicaklikYuksekAlan+SicaklikCokYuksekAlan);
    SicaklikCokDusukAlan=0;
    SicaklikDusukAlan=0;
    SicaklikOrtaAlan=0;
    SicaklikYuksekAlan=0;
    SicaklikCokYuksekAlan=0;
    return a;
}

float FuzzyLogic::water_defuzzy(float x,float y,float z,float t) {
    float a=0;
    float b=0;
    float c=0;
    float d=0;
    if (strcmp(water,water1)==0)
    {
       x=max(x,y);
       y=0;
    }
     if (strcmp(water,water2)==0)
    {
       x=max(x,z);
       z=0;
    }
     if (strcmp(water,water3)==0)
    {
        x=max(x,t);
        t=0;
    }
     if (strcmp(water1,water2)==0)
    {
        y=max(y,z);
        z=0;
    }
     if (strcmp(water1,water3)==0)
    {
       y=max(y,t);
       t=0;
    }
     if (strcmp(water2,water3)==0)
    {
        z=max(z,t);
        t=0;
    }
    if (strcmp(water1,"cok_dusuk")==0 && y!=0)
    {
    d=SulamaDurulastirmaCokDusuk(y);
    }
    else if (strcmp(water1,"dusuk")==0 && y!=0)
    {
    d=SulamaDurulastirmaDusuk(y);
    }
    else if (strcmp(water1,"orta")==0 && y!=0)
    {
    d=SulamaDurulastirmaOrta(y);
    }
    else if (strcmp(water1,"yuksek")==0 && y!=0)
    {
    d=SulamaDurulastirmaYuksek(y);
    }
    else if (strcmp(water1,"cok_yuksek")==0 && y!=0)
    {
    d=SulamaDurulastirmaCokYuksek(y);
    }
    if (strcmp(water,"cok_dusuk")==0 && x!=0)
    {
    a=SulamaDurulastirmaCokDusuk(x);
    }
    else if (strcmp(water,"dusuk")==0 && x!=0)
    {
    a=SulamaDurulastirmaDusuk(x);
    }
    else if (strcmp(water,"orta")==0 && x!=0)
    {
    a=SulamaDurulastirmaOrta(x);
    }
    else if (strcmp(water,"yuksek")==0 && x!=0)
    {
    a=SulamaDurulastirmaYuksek(x);
    }
    else if (strcmp(water,"cok_yuksek")==0 && x!=0)
    {
    a=SulamaDurulastirmaCokYuksek(x);
    }
    if (strcmp(water2,"cok_dusuk")==0 && z!=0)
    {
    b=SulamaDurulastirmaCokDusuk(z);
    }
    else if (strcmp(water2,"dusuk")==0 && z!=0)
    {
    b=SulamaDurulastirmaDusuk(z);
    }
    else if (strcmp(water2,"orta")==0 && z!=0)
    {
    b=SulamaDurulastirmaOrta(z);
    }
    else if (strcmp(water2,"yuksek")==0 && z!=0)
    {
    b=SulamaDurulastirmaYuksek(z);
    }
    else if (strcmp(water2,"cok_yuksek")==0 && z!=0)
    {
    b=SulamaDurulastirmaCokYuksek(z);
    }
    if (strcmp(water3,"cok_dusuk")==0 && t!=0)
    {
    c=SulamaDurulastirmaCokDusuk(t);
    }
    else if (strcmp(water3,"dusuk")==0 && t!=0)
    {
    c=SulamaDurulastirmaDusuk(t);
    }
    else if (strcmp(water3,"orta")==0 && t!=0)
    {
    c=SulamaDurulastirmaOrta(t);
    }
    else if (strcmp(water3,"yuksek")==0 && t!=0)
    {
    c=SulamaDurulastirmaYuksek(t);
    }
    else if (strcmp(water3,"cok_yuksek")==0 && t!=0)
    {
    c=SulamaDurulastirmaCokYuksek(t);
    }
    a=(a+b+c+d)/(SulamaCokDusukAlan+SulamaDusukAlan+SulamaOrtaAlan+SulamaYuksekAlan+SulamaCokYuksekAlan);
    SulamaCokDusukAlan=0;
    SulamaDusukAlan=0;
    SulamaOrtaAlan=0;
    SulamaYuksekAlan=0;
    SulamaCokYuksekAlan=0;
    return a;
}

float FuzzyLogic::light_defuzzy(float primary_mu_light, char* primary_label_light){
    float a = 0;
    if (strcmp(primary_label_light,"cok_dusuk")==0 )
    {
        a = IsiklandirmaDurulastirmaCokDusuk(primary_mu_light);
    }
    else if (strcmp(primary_label_light,"dusuk")==0 )
    {
        a = IsiklandirmaDurulastirmaDusuk(primary_mu_light);
    }
    else if (strcmp(primary_label_light,"orta")==0 )
    {
        a = IsiklandirmaDurulastirmaOrta(primary_mu_light);
    }
    else if (strcmp(primary_label_light,"yuksek")==0 )
    {
        a = IsiklandirmaDurulastirmaYuksek(primary_mu_light);
    }
    else if (strcmp(primary_label_light,"cok_yuksek")==0 )
    {
        a = IsiklandirmaDurulastirmaCokYuksek(primary_mu_light);
    }
    a = a/IsiklandirmaAlan;
    IsiklandirmaAlan = 0;
    return a;
}

SystemDecisions FuzzyLogic::defuzzify(FuzzyResults fuzzy_results) {
    SystemDecisions decisions;

    temp_humidity_prim_sec=min(fuzzy_results.temp_result.primary_mu, fuzzy_results.hum_result.secondary_mu);
    temp_humidity_prim_prim=min(fuzzy_results.temp_result.primary_mu, fuzzy_results.hum_result.primary_mu);
    temp_humidity_sec_sec=min(fuzzy_results.temp_result.secondary_mu, fuzzy_results.hum_result.secondary_mu);
    temp_humidity_sec_prim=min(fuzzy_results.temp_result.secondary_mu, fuzzy_results.hum_result.primary_mu);

    temp_light_prim_sec=min(fuzzy_results.temp_result.primary_mu, fuzzy_results.light_result.secondary_mu);
    temp_light_prim_prim=min(fuzzy_results.temp_result.primary_mu, fuzzy_results.light_result.primary_mu);
    temp_light_sec_sec=min(fuzzy_results.temp_result.secondary_mu, fuzzy_results.light_result.secondary_mu);
    temp_light_sec_prim=min(fuzzy_results.temp_result.secondary_mu, fuzzy_results.light_result.primary_mu);

    humidity_soil_prim_sec=min(fuzzy_results.hum_result.primary_mu, fuzzy_results.soil_result.secondary_mu);
    humidity_soil_prim_prim=min(fuzzy_results.hum_result.primary_mu, fuzzy_results.soil_result.primary_mu);
    humidity_soil_sec_sec=min(fuzzy_results.hum_result.secondary_mu, fuzzy_results.soil_result.secondary_mu);
    humidity_soil_sec_prim=min(fuzzy_results.hum_result.secondary_mu, fuzzy_results.soil_result.primary_mu);

    evaluate_rules_temp_hum_heating(fuzzy_results.temp_result.primary_label, fuzzy_results.hum_result.primary_label);
    strcpy(decisions.output_label_heat, cikis);
    strcpy(cikis1,cikis);
    evaluate_rules_temp_hum_heating(fuzzy_results.temp_result.primary_label, fuzzy_results.hum_result.secondary_label);
    strcpy(cikis2,cikis);
    evaluate_rules_temp_hum_heating(fuzzy_results.temp_result.secondary_label, fuzzy_results.hum_result.primary_label);
    strcpy(cikis3,cikis);
    evaluate_rules_temp_hum_heating(fuzzy_results.temp_result.secondary_label, fuzzy_results.hum_result.secondary_label);

    evaluate_rules_temp_hum_cooling(fuzzy_results.temp_result.primary_label, fuzzy_results.hum_result.primary_label);
    strcpy(decisions.output_label_cooling, Cooling);
    strcpy(Cooling1,Cooling);
    evaluate_rules_temp_hum_cooling(fuzzy_results.temp_result.primary_label, fuzzy_results.hum_result.secondary_label);
    strcpy(Cooling2,Cooling);
    evaluate_rules_temp_hum_cooling(fuzzy_results.temp_result.secondary_label, fuzzy_results.hum_result.primary_label);
    strcpy(Cooling3,Cooling);
    evaluate_rules_temp_hum_cooling(fuzzy_results.temp_result.secondary_label, fuzzy_results.hum_result.secondary_label);

    evaluate_rules_temp_light_shadow(fuzzy_results.temp_result.primary_label, fuzzy_results.light_result.primary_label);
    strcpy(decisions.output_label_shadow, shadow);
    strcpy(shadow1,shadow);
    evaluate_rules_temp_light_shadow(fuzzy_results.temp_result.primary_label, fuzzy_results.light_result.secondary_label);
    strcpy(shadow2,shadow);
    evaluate_rules_temp_light_shadow(fuzzy_results.temp_result.secondary_label, fuzzy_results.light_result.primary_label);
    strcpy(shadow3,shadow);
    evaluate_rules_temp_light_shadow(fuzzy_results.temp_result.secondary_label, fuzzy_results.light_result.secondary_label);

    evaluate_rules_hum_soil(fuzzy_results.hum_result.primary_label, fuzzy_results.soil_result.primary_label);
    strcpy(decisions.output_label_water, water);
    strcpy(water1,water);
    evaluate_rules_hum_soil(fuzzy_results.hum_result.primary_label, fuzzy_results.soil_result.secondary_label);
    strcpy(water2,water);
    evaluate_rules_hum_soil(fuzzy_results.hum_result.secondary_label, fuzzy_results.soil_result.primary_label);
    strcpy(water3,water);
    evaluate_rules_hum_soil(fuzzy_results.hum_result.secondary_label, fuzzy_results.soil_result.secondary_label);

    evaluate_rules_temp_light_shadow(fuzzy_results.temp_result.primary_label, fuzzy_results.light_result.primary_label);
    strcpy(decisions.output_label_shadow, shadow);

    evaluate_rules_hum_soil(fuzzy_results.hum_result.primary_label, fuzzy_results.soil_result.primary_label);
    strcpy(decisions.output_label_water, water);

    evaluate_rules_light(fuzzy_results.light_result.primary_label, &decisions);

    decisions.output_value_heat = heat_defuzzy(temp_humidity_prim_sec, temp_humidity_prim_prim, temp_humidity_sec_sec, temp_humidity_sec_prim);
    decisions.output_value_cooling = cooling_defuzzy(temp_humidity_prim_sec, temp_humidity_prim_prim, temp_humidity_sec_sec, temp_humidity_sec_prim);
    decisions.output_value_shadow = shadow_defuzzy(temp_light_prim_sec,temp_light_prim_prim, temp_light_sec_sec, temp_light_sec_prim);
    decisions.output_value_water = water_defuzzy(humidity_soil_prim_sec, humidity_soil_prim_prim, humidity_soil_sec_sec, humidity_soil_sec_prim);
    decisions.output_value_light = light_defuzzy(fuzzy_results.light_result.primary_mu, decisions.output_label_light);
    
    return decisions;
}