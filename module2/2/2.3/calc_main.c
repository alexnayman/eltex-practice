#include <stdio.h>
#include "calc_funcs.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    Operation ops[] = {
        {'+', add, 0},
        {'-', sub, 0},
        {'*', mul, 0},
        {'/', dv,  0},
        {'2', sqr, 1},
        {'i', inv, 1} 
    };
    int n_ops = sizeof(ops) / sizeof(ops[0]);

    char sym;
    double a, b;

    while (1) {
        printf("\nДействие (+ - * / 2-квадрат i-инверсия) или 0-выход: ");
        if (scanf(" %c", &sym) != 1 || sym == '0') break;

        int found = -1;
        for (int i = 0; i < n_ops; i++) {
            if (ops[i].op == sym) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf("! Ошибка: неизвестная операция\n");
            clean();
            continue;
        }

        if (ops[found].is_unary) {
            printf("Введите число: ");
            if (scanf("%lf", &a) != 1) {
                printf("! Ошибка ввода\n");
                clean();
                continue;
            }
            printf("Результат: %.2f\n", ops[found].func(a, 0));
        } else {
            printf("Введите два числа: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("! Ошибка ввода\n");
                clean();
                continue;
            }
            printf("Результат: %.2f\n", ops[found].func(a, b));
        }
    }

    return 0;
}