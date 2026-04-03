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

void show_all(Node* root, int* i) {
    if (!root) return;
    show_all(root->left, i);
    show_one(&root->data, (*i)++);
    show_all(root->right, i);
}

Node* add_node(Node* root, Contact c) {
    if (!root) {
        Node* n = malloc(sizeof(Node));
        n->data = c; n->left = n->right = NULL;
        return n;
    }
    if (strcmp(c.name, root->data.name) < 0) root->left = add_node(root->left, c);
    else root->right = add_node(root->right, c);
    return root;
}

Node* find_by_name(Node* root, const char* s, Node** res_arr, int* cnt) {
    if (!root) return NULL;
    find_by_name(root->left, s, res_arr, cnt);
    if (strstr(root->data.name, s)) res_arr[(*cnt)++] = root;
    find_by_name(root->right, s, res_arr, cnt);
    return NULL;
}

Node* find_by_phone(Node* root, const char* s, Node** res_arr, int* cnt) {
    if (!root) return NULL;
    find_by_phone(root->left, s, res_arr, cnt);
    if (strstr(root->data.phone, s)) res_arr[(*cnt)++] = root;
    find_by_phone(root->right, s, res_arr, cnt);
    return NULL;
}

Node* find_by_idx(Node* root, int target, int* curr) {
    if (!root) return NULL;
    Node* l = find_by_idx(root->left, target, curr);
    if (l) return l;
    if (++(*curr) == target) return root;
    return find_by_idx(root->right, target, curr);
}

Node* min_node(Node* n) {
    while (n && n->left) n = n->left;
    return n;
}

Node* delete_node(Node* root, const char* name) {
    if (!root) return NULL;
    int cmp = strcmp(name, root->data.name);
    if (cmp < 0) root->left = delete_node(root->left, name);
    else if (cmp > 0) root->right = delete_node(root->right, name);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            free(root->data.name); free(root->data.phone); free(root->data.work);
            free(root->data.job); free(root->data.mail); free(root->data.soc);
            free(root); return temp;
        }
        Node* temp = min_node(root->right);
        Contact swap = root->data; root->data = temp->data; temp->data = swap;
        root->right = delete_node(root->right, temp->data.name);
    }
    return root;
}

void edit_node(Node* target, int num, ...) {
    va_list args; va_start(args, num);
    for (int j = 0; j < num; j++) {
        int f = va_arg(args, int);
        printf("Поле %d: ", f);
        char *v = read_s(f == 1 || f == 4);
        if (f == 1) { free(target->data.name); target->data.name = v; }
        else if (f == 2) { free(target->data.work); target->data.work = v; }
        else if (f == 3) { free(target->data.job); target->data.job = v; }
        else if (f == 4) { free(target->data.phone); target->data.phone = v; }
        else if (f == 5) { free(target->data.mail); target->data.mail = v; }
        else if (f == 6) { free(target->data.soc); target->data.soc = v; }
    }
    va_end(args);
}

void tree_to_arr(Node* root, Node** arr, int* i) {
    if (!root) return;
    tree_to_arr(root->left, arr, i);
    arr[(*i)++] = root;
    tree_to_arr(root->right, arr, i);
}

Node* build_bal(Node** arr, int s, int e) {
    if (s > e) return NULL;
    int m = (s + e) / 2;
    Node* root = arr[m];
    root->left = build_bal(arr, s, m - 1);
    root->right = build_bal(arr, m + 1, e);
    return root;
}

Node* balance_tree(Node* root, int n) {
    if (!root) return NULL;
    Node** arr = malloc(sizeof(Node*) * n);
    int i = 0; tree_to_arr(root, arr, &i);
    Node* res = build_bal(arr, 0, n - 1);
    free(arr); return res;
}

void free_tree(Node* root) {
    if (!root) return;
    free_tree(root->left); free_tree(root->right);
    free(root->data.name); free(root->data.phone); free(root->data.work);
    free(root->data.job); free(root->data.mail); free(root->data.soc);
    free(root);
}
