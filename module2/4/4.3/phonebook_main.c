#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phonebook_funcs.h"

Node* find_logic(Node* root, int n) {
    if (!root) { printf("Пусто\n"); return NULL; }
    printf("1-ФИО, 2-Тел, 3-№: ");
    int m; scanf("%d", &m); clean();
    if (m == 3) {
        int idx, curr = 0; printf("№: "); scanf("%d", &idx); clean();
        return find_by_idx(root, idx, &curr);
    }
    printf("Запрос: ");
    char s[256]; fgets(s, 256, stdin); s[strcspn(s, "\n")] = 0;
    Node* res_arr[1024]; int cnt = 0;
    if (m == 1) find_by_name(root, s, res_arr, &cnt);
    else find_by_phone(root, s, res_arr, &cnt);
    if (!cnt) return NULL;
    if (cnt == 1) return res_arr[0];
    for (int i = 0; i < cnt; i++) printf("[%d] %s\n", i + 1, res_arr[i]->data.name);
    printf("Выбери № из списка: ");
    int c; scanf("%d", &c); clean();
    return (c > 0 && c <= cnt) ? res_arr[c - 1] : NULL;
}

int main() {
    Node* root = NULL; int count = 0, c;
    while (1) {
        printf("\n1.Список 2.Добавить 3.Поиск 4.Удалить 5.Ред 0.Выход\n> ");
        if (scanf("%d", &c) != 1) { clean(); continue; }
        clean();
        if (c == 0) break;
        if (c == 1) { int i = 0; show_all(root, &i); }
        if (c == 2) {
            Contact n; printf("Тел: "); n.phone = read_s(1);
            Node* check_arr[1]; int check_cnt = 0;
            find_by_phone(root, n.phone, check_arr, &check_cnt);
            if (check_cnt > 0) { printf("Номер уже существует!\n"); free(n.phone); continue; }
            printf("ФИО: "); n.name = read_s(1);
            printf("Работа: "); n.work = read_s(0);
            printf("Должность: "); n.job = read_s(0);
            printf("Email: "); n.mail = read_s(0);
            printf("Соцсети: "); n.soc = read_s(0);
            root = add_node(root, n); count++;
            if (count % 5 == 0) root = balance_tree(root, count);
        }
        if (c == 3) { Node* res = find_logic(root, count); if (res) show_one(&res->data, 0); }
        if (c == 4) {
            Node* res = find_logic(root, count);
            if (res) {
                printf("Удалить? (1-Да, 0-Нет): ");
                int conf; scanf("%d", &conf); clean();
                if (conf == 1) { root = delete_node(root, res->data.name); count--; }
            }
        }
        if (c == 5) {
            Node* res = find_logic(root, count);
            if (res) {
                printf("Поля 1-ФИО, 2-Раб, 3-Долж, 4-Тел, 5-Email, 6-Соцсети\nЦифры (124): ");
                char s[10]; scanf("%9s", s); clean();
                for (int j = 0; s[j]; j++) edit_node(res, 1, s[j] - '0');
            }
        }
    }
    free_tree(root);
    return 0;
}
