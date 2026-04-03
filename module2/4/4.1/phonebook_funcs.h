#ifndef FUNCS_H
#define FUNCS_H

#include <stdarg.h>

typedef struct {
    char *name, *work, *job, *phone, *mail, *soc;
} Contact;

typedef struct Node {
    Contact data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    char op;
    char* (*get_field)(Contact*);
} SearchMap;

void clean();
char* read_s(int req);
Node* add_ordered(Node *head);
void show_one(Contact* c, int i);
void show_all(Node *head);
Node* find_node(Node *head);
Node* del_node(Node *head, Node *target);
void edit_node(Node *target, int num, ...);
void free_list(Node *head);

#endif
