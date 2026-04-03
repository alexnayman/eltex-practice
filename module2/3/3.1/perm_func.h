#ifndef PERM_FUNC_H
#define PERM_FUNC_H

#include <sys/types.h>

void p_bin(mode_t m);
void p_sym(mode_t m);
void show_perm(mode_t m);
mode_t get_sym(const char* s);
mode_t get_oct(const char* s);
mode_t mod_perm(mode_t c, const char* s);

#endif
