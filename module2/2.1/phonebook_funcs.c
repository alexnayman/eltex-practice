#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phonebook_funcs.h"

void clean() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char* read_s(int req) {
    char buf[1024];
    while (1) {
        if (!fgets(buf, 1024, stdin)) return NULL;
        buf[strcspn(buf, "\n")] = 0;
        if (req && strlen(buf) == 0) {
            printf("! Обязательно: ");
            continue;
        }
        return strlen(buf) ? strdup(buf) : NULL;
    }
}

void show_one(Contact* c, int i) {
    printf("\n{%d} ФИО: %s\nТел: %s\nРаб: %s (%s)\nEmail: %s\nСоцсети: %s\n", 
           i + 1, c->name, c->phone, c->work ? c->work : "-", 
           c->job ? c->job : "-", c->mail ? c->mail : "-", c->soc ? c->soc : "-");
}

Contact* add(Contact* b, int *n, int *cap) {
    printf("Тел: ");
    char *p = read_s(1);
    for (int i = 0; i < *n; i++) {
        if (strcmp(b[i].phone, p) == 0) {
            printf("Номер уже существует!\n");
            free(p);
            return b;
        }
    }
    if (*n >= *cap) {
        *cap *= 2;
        b = realloc(b, sizeof(Contact) * (*cap));
    }
    b[*n].phone = p;
    printf("ФИО: "); b[*n].name = read_s(1);
    printf("Работа: "); b[*n].work = read_s(0);
    printf("Должность: "); b[*n].job = read_s(0);
    printf("Email: "); b[*n].mail = read_s(0);
    printf("Соцсети: "); b[*n].soc = read_s(0);
    (*n)++;
    return b;
}

void show_all(Contact* b, int n) {
    if (!n) { printf("Пусто\n"); return; }
    for (int i = 0; i < n; i++) show_one(&b[i], i);
}

int find(Contact* b, int n) {
    if (!n) { printf("Пусто\n"); return -1; }
    printf("1-ФИО, 2-Тел, 3-№: ");
    int m, cnt = 0, res[n];
    scanf("%d", &m); clean();
    if (m == 3) {
        int i; printf("№: "); scanf("%d", &i); clean();
        return (i > 0 && i <= n) ? i - 1 : -1;
    }
    printf("Запрос: ");
    char s[256]; fgets(s, 256, stdin); s[strcspn(s, "\n")] = 0;
    for (int i = 0; i < n; i++) {
        char *t = (m == 1) ? b[i].name : b[i].phone;
        if (t && strstr(t, s)) {
            printf("[%d] %s\n", i + 1, b[i].name);
            res[cnt++] = i;
        }
    }
    if (!cnt) return -1;
    if (cnt == 1) return res[0];
    printf("Выбери № из списка: ");
    int c; scanf("%d", &c); clean();
    return c - 1;
}

void edit(Contact* b, int n) {
    int i = find(b, n);
    if (i == -1) return;
    printf("Поля 1-ФИО, 2-Раб, 3-Долж, 4-Тел, 5-Email, 6-Соцсети\nЦифры (124): ");
    char s[10]; scanf("%9s", s); clean();
    for (int j = 0; s[j]; j++) {
        int f = s[j] - '0';
        printf("Поле %d: ", f);
        char *v = read_s(f == 1 || f == 4);
        if (f == 1) { free(b[i].name); b[i].name = v; }
        else if (f == 2) { free(b[i].work); b[i].work = v; }
        else if (f == 3) { free(b[i].job); b[i].job = v; }
        else if (f == 4) { free(b[i].phone); b[i].phone = v; }
        else if (f == 5) { free(b[i].mail); b[i].mail = v; }
        else if (f == 6) { free(b[i].soc); b[i].soc = v; }
    }
}

void del(Contact** b, int *n) {
    int i = find(*b, *n);
    if (i == -1) return;
    printf("Удалить? (1-Да, 0-Нет): ");
    int c; scanf("%d", &c); clean();
    if (c == 1) {
        free((*b)[i].name); free((*b)[i].phone); 
        free((*b)[i].work); free((*b)[i].job);
        free((*b)[i].mail); free((*b)[i].soc);
        for (int j = i; j < *n - 1; j++) (*b)[j] = (*b)[j + 1];
        (*n)--;
    }
}

void free_b(Contact* b, int n) {
    for (int i = 0; i < n; i++) {
        free(b[i].name); free(b[i].phone);
        free(b[i].work); free(b[i].job);
        free(b[i].mail); free(b[i].soc);
    }
    free(b);
}