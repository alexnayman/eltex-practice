#include <stdio.h>
#include "calc_funcs.h"

void clean() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mul(double a, double b) { return a * b; }

double dv(double a, double b) {
    if (b == 0) {
        printf("! Ошибка: деление на ноль\n");
        return 0;
    }
    return a / b;
}

double sqr(double a, double b) { 
    return a * a; 
}

double inv(double a, double b) {
    if (a == 0) {
        printf("! Ошибка: возведение 0 в степень -1 невозможно\n");
        return 0;
    }
    return 1.0 / a;
}