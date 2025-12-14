#include "fuzzy_functions.h"
#include <math.h>

double min(double a, double b) {
    return (a < b) ? a : b;
}
double max(double a, double b) {
    return (a > b) ? a : b;
}

double trapmf2yukselen(double x, double a, double b, double c, double d) {
    double mu_yukselen=0; ;
    if (b<=x&&x<=c)
    {
      mu_yukselen=1;
    }
    else mu_yukselen=(x-a)/(b-a);
    if (mu_yukselen<0)
    {
       mu_yukselen=0;
    }
    if (mu_yukselen>1)
    {
        mu_yukselen=1;
    }
    return mu_yukselen;
}

double trapmf2dusen(double x, double a, double b, double c, double d) {
    double mu_dusen =0;
    if (a<=x && x<=c)  
    {
         mu_dusen=1;
    }
     else mu_dusen=(d-x)/(d-c);
     if (mu_dusen<0)
    {
       mu_dusen=0;
    }
     if (mu_dusen>1)
    {
        mu_dusen=1;
    }
    return mu_dusen;
}

double trimf(double x, double a, double b, double c) {
    double mu;
    if (x <= a || x >= c) {
        mu = 0.0;
    } else if (x >= a && x <= b) {
        mu = (x - a) / (b - a); 
    } else if (x >= b && x <= c) {
        mu = (c - x) / (c - b); 
    } else {
        mu = 0.0;
    }
    return max(0.0, min(1.0, mu));
}

FuzzyInput fuzzify_temperature_final(double temp) {
    FuzzyInput temp_mu;
    temp_mu.cok_dusuk  = trapmf2dusen(temp, -10.0, -10.0, 0.0, 10.0);
    temp_mu.dusuk      = trimf(temp, 0.0, 7.5, 15.0);
    temp_mu.orta       = trimf(temp, 14.0, 20.0, 26.0);
    temp_mu.yuksek     = trimf(temp, 20.0, 30.0, 40.0);
    temp_mu.cok_yuksek = trapmf2yukselen(temp, 30.0, 40.0, 50.0, 50.0);
    return temp_mu;
}

void print_fuzzification_results(double temp) {
    FuzzyInput fi = fuzzify_temperature_final(temp);
    printf(">>> SICAKLIK BULANIKLAŞTIRMA (Giriş: %.1f°C) <<<\n", temp);
    printf("1. ÇOK DÜŞÜK (mu_ÇD):  %.4f\n", fi.cok_dusuk);
    printf("2. DÜŞÜK (mu_D):       %.4f\n", fi.dusuk);
    printf("3. ORTA (mu_O):        %.4f\n", fi.orta);
    printf("4. YÜKSEK (mu_Y):      %.4f\n", fi.yuksek);
    printf("5. ÇOK YÜKSEK (mu_ÇY): %.4f\n", fi.cok_yuksek);
    printf("---------------------------------------------------\n");
}

FuzzyInput fuzzify_huminidty_final(double humin) {
    FuzzyInput nem_mu;
    nem_mu.cok_dusuk  = trapmf2dusen(humin, 0.0, 0.0, 20.0, 40.0);
    nem_mu.dusuk      = trimf(humin, 20.0, 39.5, 59.0);
    nem_mu.orta       = trimf(humin, 50.0, 60.0, 70.0);
    nem_mu.yuksek     = trimf(humin, 60.0, 75.0, 90.0);
    nem_mu.cok_yuksek = trapmf2yukselen(humin, 80.0, 90.0, 100.0, 100.0);
    return nem_mu;
}

void print_fuzzification_results2(double humin) {
    FuzzyInput nem = fuzzify_huminidty_final(humin);
    printf(">>> HAVA NEMİ BULANIKLAŞTIRMA (Giriş: %.1fnem) <<<\n", humin);
    printf("1. ÇOK DÜŞÜK (mu_ÇD):  %.4f\n", nem.cok_dusuk);
    printf("2. DÜŞÜK (mu_D):       %.4f\n", nem.dusuk);
    printf("3. ORTA (mu_O):        %.4f\n", nem.orta);
    printf("4. YÜKSEK (mu_Y):      %.4f\n", nem.yuksek);
    printf("5. ÇOK YÜKSEK (mu_ÇY): %.4f\n", nem.cok_yuksek);
    printf("---------------------------------------------------\n");
}

FuzzyInput fuzzify_isik_final(double isiksiddeti) {
    FuzzyInput isik_mu;
    isik_mu.cok_dusuk  = trapmf2dusen(isiksiddeti, 0.0, 0.0, 2500.0, 5000.0);
    isik_mu.dusuk      = trimf(isiksiddeti, 4000.0, 6500.0, 9000.0);
    isik_mu.orta       = trimf(isiksiddeti, 8000.0, 10000.0, 12000.0);
    isik_mu.yuksek     = trimf(isiksiddeti, 11000.0, 13500.0, 16000.0);
    isik_mu.cok_yuksek = trapmf2yukselen(isiksiddeti, 15000.0, 17500.0, 20000.0, 20000.0);
    return isik_mu;
}

void print_fuzzification_results3(double isiksiddeti) {
    FuzzyInput isik = fuzzify_isik_final(isiksiddeti);
    printf(">>> IŞIK ŞİDDETİ BULANIKLAŞTIRMA (Giriş: %.1fişikşiddeti) <<<\n", isiksiddeti);
    printf("1. ÇOK DÜŞÜK (mu_ÇD):  %f\n", isik.cok_dusuk);
    printf("2. DÜŞÜK (mu_D):       %f\n", isik.dusuk);
    printf("3. ORTA (mu_O):        %f\n", isik.orta);
    printf("4. YÜKSEK (mu_Y):      %f\n", isik.yuksek);
    printf("5. ÇOK YÜKSEK (mu_ÇY): %f\n", isik.cok_yuksek);
    printf("---------------------------------------------------\n");
}

FuzzyInput fuzzify_topraknemi_final(double topraknemi) {
    FuzzyInput topnemi_mu;
    topnemi_mu.cok_dusuk  = trapmf2dusen(topraknemi, 0.0, 0.0, 20.0, 40.0);
    topnemi_mu.dusuk      = trimf(topraknemi, 30.0, 49.5, 69.0);
    topnemi_mu.orta       = trimf(topraknemi, 60.0, 69.5, 79.0);
    topnemi_mu.yuksek     = trimf(topraknemi, 70.0, 80.0, 90.0);
    topnemi_mu.cok_yuksek = trapmf2yukselen(topraknemi, 80.0, 90.0, 100.0, 100.0);
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