#ifndef FUNCS_H
#define FUNCS_H

typedef struct {
    char *name, *work, *job, *phone, *mail, *soc;
} Contact;

void clean();
char* read_s(int req);
Contact* add(Contact* b, int *n, int *cap);
void show_one(Contact* c, int i);
void show_all(Contact* b, int n);
int find(Contact* b, int n);
void del(Contact** b, int *n);
void edit(Contact* b, int n);
void free_b(Contact* b, int n);

#endif
