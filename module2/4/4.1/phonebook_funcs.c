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
            printf("! Обязательно: "); continue;
        }
        return strlen(buf) ? strdup(buf) : NULL;
    }
}

void show_one(Contact* c, int i) {
    printf("\n{%d} ФИО: %s\nТел: %s\nРаб: %s (%s)\nEmail: %s\nСоцсети: %s\n", 
           i + 1, c->name, c->phone, c->work ? c->work : "-", 
           c->job ? c->job : "-", c->mail ? c->mail : "-", c->soc ? c->soc : "-");
}

Node* add_ordered(Node *head) {
    Node *new_n = malloc(sizeof(Node));
    printf("Тел: "); new_n->data.phone = read_s(1);
    
    Node *temp = head;
    while(temp) {
        if (strcmp(temp->data.phone, new_n->data.phone) == 0) {
            printf("Номер уже существует!\n");
            free(new_n->data.phone); free(new_n);
            return head;
        }
        temp = temp->next;
    }

    printf("ФИО: "); new_n->data.name = read_s(1);
    printf("Работа: "); new_n->data.work = read_s(0);
    printf("Должность: "); new_n->data.job = read_s(0);
    printf("Email: "); new_n->data.mail = read_s(0);
    printf("Соцсети: "); new_n->data.soc = read_s(0);
    new_n->next = new_n->prev = NULL;

    if (!head) return new_n;

    Node *curr = head;
    while (curr->next && strcmp(curr->data.name, new_n->data.name) < 0) {
        curr = curr->next;
    }

    if (strcmp(curr->data.name, new_n->data.name) >= 0) {
        new_n->next = curr;
        new_n->prev = curr->prev;
        if (curr->prev) curr->prev->next = new_n;
        curr->prev = new_n;
        return (curr == head) ? new_n : head;
    } else {
        new_n->prev = curr;
        curr->next = new_n;
        return head;
    }
}

void show_all(Node *head) {
    if (!head) { printf("Пусто\n"); return; }
    Node *curr = head;
    int i = 0;
    while (curr) {
        show_one(&curr->data, i++);
        curr = curr->next;
    }
}

char* get_n(Contact *c) { return c->name; }
char* get_p(Contact *c) { return c->phone; }

Node* find_node(Node *head) {
    if (!head) { printf("Пусто\n"); return NULL; }
    printf("1-ФИО, 2-Тел, 3-№: ");
    int m; scanf("%d", &m); clean();
    
    if (m == 3) {
        int target_idx, i = 0;
        printf("№: "); scanf("%d", &target_idx); clean();
        Node *curr = head;
        while (curr && ++i < target_idx) curr = curr->next;
        return curr;
    }

    printf("Запрос: ");
    char s[256]; fgets(s, 256, stdin); s[strcspn(s, "\n")] = 0;
    
    Node *results[100]; // Массив для выбора из найденных
    int cnt = 0, idx = 1;
    Node *curr = head;
    
    while (curr) {
        char *val = (m == 1) ? curr->data.name : curr->data.phone;
        if (val && strstr(val, s)) {
            printf("[%d] %s (%s)\n", idx, curr->data.name, curr->data.phone);
            results[cnt++] = curr;
        }
        curr = curr->next;
        idx++;
    }

    if (cnt == 0) return NULL;
    if (cnt == 1) return results[0];
    
    printf("Выбери № из списка: ");
    int choice; scanf("%d", &choice); clean();
    return (choice > 0 && choice <= cnt) ? results[choice-1] : NULL;
}

void edit_node(Node *target, int num, ...) {
    if (!target) return;
    va_list args;
    va_start(args, num);
    
    for (int j = 0; j < num; j++) {
        int f = va_arg(args, int);
        
        char *field_name = "";
        if (f == 1) field_name = "ФИО";
        else if (f == 2) field_name = "Работа";
        else if (f == 3) field_name = "Должность";
        else if (f == 4) field_name = "Телефон";
        else if (f == 5) field_name = "Email";
        else if (f == 6) field_name = "Соцсети";

        printf("[%s] Новое значение: ", field_name);
        char *v = read_s(f == 1 || f == 4);

        if (f == 1)      { free(target->data.name);  target->data.name = v; }
        else if (f == 2) { free(target->data.work);  target->data.work = v; }
        else if (f == 3) { free(target->data.job);   target->data.job = v; }
        else if (f == 4) { free(target->data.phone); target->data.phone = v; }
        else if (f == 5) { free(target->data.mail);  target->data.mail = v; }
        else if (f == 6) { free(target->data.soc);   target->data.soc = v; }
    }
    va_end(args);
}

Node* del_node(Node *head, Node *t) {
    if (!head || !t) return head;
    printf("Удалить? (1-Да, 0-Нет): ");
    int c; scanf("%d", &c); clean();
    if (c != 1) return head;

    if (t->prev) t->prev->next = t->next;
    if (t->next) t->next->prev = t->prev;
    Node *new_head = (t == head) ? t->next : head;

    free(t->data.name); free(t->data.phone); 
    free(t->data.work); free(t->data.job);
    free(t->data.mail); free(t->data.soc);
    free(t);
    return new_head;
}

void free_list(Node *head) {
    while (head) {
        Node *temp = head;
        head = head->next;
        free(temp->data.name); free(temp->data.phone);
        free(temp->data.work); free(temp->data.job);
        free(temp->data.mail); free(temp->data.soc);
        free(temp);
    }
}
