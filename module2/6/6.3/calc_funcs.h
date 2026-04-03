#ifndef CALC_FUNCS_H
#define CALC_FUNCS_H

typedef double (*calc_func)(double, double);

typedef struct {
    char op;
    calc_func func;
    void *handle;
    int is_unary;
} Plugin;

void clean();

#endif
