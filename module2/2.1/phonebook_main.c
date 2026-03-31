#include <stdio.h>
#include <stdlib.h>
#include "phonebook_funcs.h"
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
    int n = 0, cap = 10, c;
    Contact* b = malloc(sizeof(Contact) * cap);
    while (1) {
        printf("\n1.Список 2.Добавить 3.Поиск 4.Удалить 5.Ред 0.Выход\n> ");
        if (scanf("%d", &c) != 1) { clean(); continue; }
        clean();
        if (c == 0) break;
        if (c == 1) show_all(b, n);
        if (c == 2) b = add(b, &n, &cap);
        if (c == 3) { int i = find(b, n); if(i != -1) show_one(&b[i], i); }
        if (c == 4) del(&b, &n);
        if (c == 5) edit(b, n);
    }
    free_b(b, n);
    return 0;
}