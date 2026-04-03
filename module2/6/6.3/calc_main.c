#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include "calc_funcs.h"

void clean() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    Plugin plugins[10];
    int p_count = 0;
    DIR *dir = opendir("./plugins");
    struct dirent *entry;

    if (!dir) return 1;

    while ((entry = readdir(dir)) != NULL && p_count < 10) {
        if (strstr(entry->d_name, ".so")) {
            char path[512];
            snprintf(path, sizeof(path), "./plugins/%s", entry->d_name);
            void *h = dlopen(path, RTLD_LAZY);
            if (!h) continue;

            calc_func f = (calc_func)dlsym(h, "execute");
            if (f) {
                plugins[p_count].func = f;
                plugins[p_count].handle = h;
                
                if (strcmp(entry->d_name, "add.so") == 0) plugins[p_count].op = 'a';
                else if (strcmp(entry->d_name, "sub.so") == 0) plugins[p_count].op = 's';
                else if (strcmp(entry->d_name, "mul.so") == 0) plugins[p_count].op = 'm';
                else if (strcmp(entry->d_name, "dv.so") == 0) plugins[p_count].op = 'd';
                else if (strcmp(entry->d_name, "sqr.so") == 0) plugins[p_count].op = '2';
                else if (strcmp(entry->d_name, "inv.so") == 0) plugins[p_count].op = 'i';
                else plugins[p_count].op = entry->d_name[0];

                plugins[p_count].is_unary = (plugins[p_count].op == '2' || plugins[p_count].op == 'i');
                p_count++;
            } else {
                dlclose(h);
            }
        }
    }
    closedir(dir);

    char sym;
    double a, b;

    while (1) {
        printf("\nДоступные операции:\n");
        for(int i = 0; i < p_count; i++) {
            switch(plugins[i].op) {
                case 'a': printf("  a - сложение\n"); break;
                case 's': printf("  s - вычитание\n"); break;
                case 'm': printf("  m - умножение\n"); break;
                case 'd': printf("  d - деление\n"); break;
                case '2': printf("  2 - квадрат\n"); break;
                case 'i': printf("  i - инверсия\n"); break;
            }
        }
        printf("Выберите действие (0-выход): ");
        if (scanf(" %c", &sym) != 1 || sym == '0') break;

        int idx = -1;
        for (int i = 0; i < p_count; i++) {
            if (plugins[i].op == sym) {
                idx = i;
                break;
            }
        }

        if (idx == -1) {
            printf("! Ошибка\n");
            clean();
            continue;
        }

        if (plugins[idx].is_unary) {
            printf("Число: ");
            if (scanf("%lf", &a) != 1) { clean(); continue; }
            printf("Результат: %.2f\n", plugins[idx].func(a, 0));
        } else {
            printf("Два числа: ");
            if (scanf("%lf %lf", &a, &b) != 2) { clean(); continue; }
            printf("Результат: %.2f\n", plugins[idx].func(a, b));
        }
    }

    for (int i = 0; i < p_count; i++) dlclose(plugins[i].handle);
    return 0;
}
