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

    char op;
    double a, b, res;

    while (1) {
        printf("\nВведите действие (+ - * /) или 0 для выхода: ");
        if (scanf(" %c", &op) != 1) {
            clean();
            continue;
        }

        if (op == '0') break;

        if (op != '+' && op != '-' && op != '*' && op != '/') {
            printf("! Ошибка: неизвестная операция\n");
            clean();
            continue;
        }

        printf("Введите два числа: ");
        if (scanf("%lf %lf", &a, &b) != 2) {
            printf("! Ошибка ввода чисел\n");
            clean();
            continue;
        }

        switch (op) {
            case '+': printf("Результат: %.2f\n", add(a, b)); break;
            case '-': printf("Результат: %.2f\n", sub(a, b)); break;
            case '*': printf("Результат: %.2f\n", mul(a, b)); break;
            case '/': 
                if (div_op(a, b, &res)) printf("Результат: %.2f\n", res);
                else printf("! Ошибка: деление на ноль\n");
                break;
        }
    }
    return 0;
}