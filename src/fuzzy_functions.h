#ifndef FUZZY_FUNCTIONS_H
#define FUZZY_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct {
	double cok_dusuk;
	double dusuk;
	double orta;
	double yuksek;
	double cok_yuksek;
} FuzzyInput;

double min(double a, double b);
double max(double a, double b);

double trapmf2yukselen(double x, double a, double b, double c, double d);
double trapmf2dusen(double x, double a, double b, double c, double d);
double trimf(double x, double a, double b, double c);

FuzzyInput fuzzify_temperature_final(double temp);
void print_fuzzification_results(double temp);

FuzzyInput fuzzify_huminidty_final(double humin);
void print_fuzzification_results2(double humin);

FuzzyInput fuzzify_isik_final(double isiksiddeti);
void print_fuzzification_results3(double isiksiddeti);

FuzzyInput fuzzify_topraknemi_final(double topraknemi);
void print_fuzzification_results4(double topraknemi);

#ifdef __cplusplus
}
#endif

#endif // FUZZY_FUNCTIONS_H

