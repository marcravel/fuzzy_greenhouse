#include "fuzzy_functions.h"
#include <math.h>

float min(float a, float b) {
    return (a < b) ? a : b;
}
float max(float a, float b) {
    return (a > b) ? a : b;
}

float trapmf2yukselen(float x, float a, float b, float c, float d) {
    float mu_yukselen = 0.0f;
    if (b <= x && x <= c) {
        mu_yukselen = 1.0f;
    } else {
        mu_yukselen = (x - a) / (b - a);
    }
    if (mu_yukselen < 0.0f) mu_yukselen = 0.0f;
    if (mu_yukselen > 1.0f) mu_yukselen = 1.0f;
    return mu_yukselen;
}

float trapmf2dusen(float x, float a, float b, float c, float d) {
    float mu_dusen = 0.0f;
    if (a <= x && x <= c) {
        mu_dusen = 1.0f;
    } else {
        mu_dusen = (d - x) / (d - c);
    }
    if (mu_dusen < 0.0f) mu_dusen = 0.0f;
    if (mu_dusen > 1.0f) mu_dusen = 1.0f;
    return mu_dusen;
}

float trimf(float x, float a, float b, float c) {
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
    return max(0.0f, min(1.0f, mu));
}

FuzzyInput fuzzify_temperature_final(float temp) {
    FuzzyInput temp_mu;
    temp_mu.cok_dusuk  = trapmf2dusen(temp, -10.0f, -10.0f, 0.0f, 10.0f);
    temp_mu.dusuk      = trimf(temp, 0.0f, 7.5f, 15.0f);
    temp_mu.orta       = trimf(temp, 14.0f, 20.0f, 26.0f);
    temp_mu.yuksek     = trimf(temp, 20.0f, 30.0f, 40.0f);
    temp_mu.cok_yuksek = trapmf2yukselen(temp, 30.0f, 40.0f, 50.0f, 50.0f);
    return temp_mu;
}

void print_fuzzification_results(float temp) {
    FuzzyInput fi = fuzzify_temperature_final(temp);
    printf(">>> SICAKLIK BULANIKLAŞTIRMA (Giriş: %.1f°C) <<<\n", temp);
    printf("1. ÇOK DÜŞÜK (mu_ÇD):  %.4f\n", fi.cok_dusuk);
    printf("2. DÜŞÜK (mu_D):       %.4f\n", fi.dusuk);
    printf("3. ORTA (mu_O):        %.4f\n", fi.orta);
    printf("4. YÜKSEK (mu_Y):      %.4f\n", fi.yuksek);
    printf("5. ÇOK YÜKSEK (mu_ÇY): %.4f\n", fi.cok_yuksek);
    printf("---------------------------------------------------\n");
}

FuzzyInput fuzzify_huminidty_final(float humin) {
    FuzzyInput nem_mu;
    nem_mu.cok_dusuk  = trapmf2dusen(humin, 0.0f, 0.0f, 20.0f, 40.0f);
    nem_mu.dusuk      = trimf(humin, 20.0f, 39.5f, 59.0f);
    nem_mu.orta       = trimf(humin, 50.0f, 60.0f, 70.0f);
    nem_mu.yuksek     = trimf(humin, 60.0f, 75.0f, 90.0f);
    nem_mu.cok_yuksek = trapmf2yukselen(humin, 80.0f, 90.0f, 100.0f, 100.0f);
    return nem_mu;
}

void print_fuzzification_results2(float humin) {
    FuzzyInput nem = fuzzify_huminidty_final(humin);
    printf(">>> HAVA NEMİ BULANIKLAŞTIRMA (Giriş: %.1fnem) <<<\n", humin);
    printf("1. ÇOK DÜŞÜK (mu_ÇD):  %.4f\n", nem.cok_dusuk);
    printf("2. DÜŞÜK (mu_D):       %.4f\n", nem.dusuk);
    printf("3. ORTA (mu_O):        %.4f\n", nem.orta);
    printf("4. YÜKSEK (mu_Y):      %.4f\n", nem.yuksek);
    printf("5. ÇOK YÜKSEK (mu_ÇY): %.4f\n", nem.cok_yuksek);
    printf("---------------------------------------------------\n");
}

FuzzyInput fuzzify_isik_final(float isiksiddeti) {
    FuzzyInput isik_mu;
    isik_mu.cok_dusuk  = trapmf2dusen(isiksiddeti, 0.0f, 0.0f, 2500.0f, 5000.0f);
    isik_mu.dusuk      = trimf(isiksiddeti, 4000.0f, 6500.0f, 9000.0f);
    isik_mu.orta       = trimf(isiksiddeti, 8000.0f, 10000.0f, 12000.0f);
    isik_mu.yuksek     = trimf(isiksiddeti, 11000.0f, 13500.0f, 16000.0f);
    isik_mu.cok_yuksek = trapmf2yukselen(isiksiddeti, 15000.0f, 17500.0f, 20000.0f, 20000.0f);
    return isik_mu;
}

void print_fuzzification_results3(float isiksiddeti) {
    FuzzyInput isik = fuzzify_isik_final(isiksiddeti);
    printf(">>> IŞIK ŞİDDETİ BULANIKLAŞTIRMA (Giriş: %.1fişikşiddeti) <<<\n", isiksiddeti);
    printf("1. ÇOK DÜŞÜK (mu_ÇD):  %f\n", isik.cok_dusuk);
    printf("2. DÜŞÜK (mu_D):       %f\n", isik.dusuk);
    printf("3. ORTA (mu_O):        %f\n", isik.orta);
    printf("4. YÜKSEK (mu_Y):      %f\n", isik.yuksek);
    printf("5. ÇOK YÜKSEK (mu_ÇY): %f\n", isik.cok_yuksek);
    printf("---------------------------------------------------\n");
}

FuzzyInput fuzzify_topraknemi_final(float topraknemi) {
    FuzzyInput topnemi_mu;
    topnemi_mu.cok_dusuk  = trapmf2dusen(topraknemi, 0.0f, 0.0f, 20.0f, 40.0f);
    topnemi_mu.dusuk      = trimf(topraknemi, 30.0f, 49.5f, 69.0f);
    topnemi_mu.orta       = trimf(topraknemi, 60.0f, 69.5f, 79.0f);
    topnemi_mu.yuksek     = trimf(topraknemi, 70.0f, 80.0f, 90.0f);
    topnemi_mu.cok_yuksek = trapmf2yukselen(topraknemi, 80.0f, 90.0f, 100.0f, 100.0f);
    return topnemi_mu;
}

void print_fuzzification_results4(double topraknemi) {
    FuzzyInput topnemi = fuzzify_topraknemi_final(topraknemi);
    printf(">>> TOPRAK NEMİ BULANIKLAŞTIRMA (Giriş: %.1ftopraknemi) <<<\n", topraknemi);
    printf("1. ÇOK DÜŞÜK (mu_ÇD):  %f\n", topnemi.cok_dusuk);
    printf("2. DÜŞÜK (mu_D):       %f\n", topnemi.dusuk);
    printf("3. ORTA (mu_O):        %f\n", topnemi.orta);
    printf("4. YÜKSEK (mu_Y):      %f\n", topnemi.yuksek);
    printf("5. ÇOK YÜKSEK (mu_ÇY): %f\n", topnemi.cok_yuksek);
    printf("---------------------------------------------------\n");
}

// Removed `main()` from this implementation file so it can be
// linked into firmware which has a single Arduino `setup()`/`loop()`
// entry point (see `src/main.cpp`).