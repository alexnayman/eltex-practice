#include <stdio.h>
#include "calc_funcs.h"

void clean() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mul(double a, double b) { return a * b; }

int div_op(double a, double b, double *res) {
    if (b == 0) return 0;
    *res = a / b;
    return 1;
}