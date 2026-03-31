#ifndef CALC_FUNCS_H
#define CALC_FUNCS_H

typedef struct {
    char op;                   
    double (*func)(double, double); 
    int is_unary; 
} Operation;

void clean();
double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double dv(double a, double b);
double sqr(double a, double b);
double inv(double a, double b);

#endif