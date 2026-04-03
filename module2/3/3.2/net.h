#ifndef NET_H
#define NET_H

#include <stdint.h>

uint32_t p_ip(char *s);
void f_ip(uint32_t ip, char *buf);
int chk(uint32_t ip, uint32_t gw, uint32_t mk);

#endif
