#ifndef FUZZY_FUNCTIONS_H
#define FUZZY_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct {
    float cok_dusuk;
    float dusuk;
    float orta;
    float yuksek;
    float cok_yuksek;
} FuzzyInput;

float min(float a, float b);
float max(float a, float b);

float trapmf2yukselen(float x, float a, float b, float c, float d);
float trapmf2dusen(float x, float a, float b, float c, float d);
float trimf(float x, float a, float b, float c);

FuzzyInput fuzzify_temperature_final(float temp);
void print_fuzzification_results(float temp);

FuzzyInput fuzzify_huminidty_final(float humin);
void print_fuzzification_results2(float humin);

FuzzyInput fuzzify_isik_final(float isiksiddeti);
void print_fuzzification_results3(float isiksiddeti);

FuzzyInput fuzzify_topraknemi_final(float topraknemi);
void print_fuzzification_results4(float topraknemi);

#ifdef __cplusplus
}
#endif

#endif // FUZZY_FUNCTIONS_H

