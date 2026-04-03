#ifndef FUNCS_H
#define FUNCS_H

#include <stdarg.h>

typedef struct {
    char *name, *work, *job, *phone, *mail, *soc;
} Contact;

typedef struct Node {
    Contact data;
    struct Node *left;
    struct Node *right;
} Node;

void clean();
char* read_s(int req);
void show_one(Contact* c, int i);
void show_all(Node* root, int* i);

Node* add_node(Node* root, Contact c);
Node* find_by_name(Node* root, const char* s, Node** res_arr, int* cnt);
Node* find_by_phone(Node* root, const char* s, Node** res_arr, int* cnt);
Node* find_by_idx(Node* root, int target, int* curr);

Node* delete_node(Node* root, const char* name);
void edit_node(Node* target, int num, ...);

Node* balance_tree(Node* root, int n);
void free_tree(Node* root);

#endif
