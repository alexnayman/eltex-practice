#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net.h"

uint32_t p_ip(char *s) {
    uint32_t res = 0;
    char str[20];
    strcpy(str, s);
    char *t = strtok(str, ".");
    for (int i = 0; i < 4 && t != NULL; i++) {
        res |= ((uint32_t)atoi(t) << (24 - i * 8));
        t = strtok(NULL, ".");
    }
    return res;
}

void f_ip(uint32_t ip, char *buf) {
    sprintf(buf, "%u.%u.%u.%u", 
            (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, 
            (ip >> 8) & 0xFF, ip & 0xFF);
}

int chk(uint32_t ip, uint32_t gw, uint32_t mk) {
    return (ip & mk) == (gw & mk);
}
