#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "net.h"

int main(int argc, char *argv[]) {
    if (argc < 4) return 1;

    uint32_t gw = p_ip(argv[1]);
    uint32_t mk = p_ip(argv[2]);
    int n = atoi(argv[3]);
    int loc = 0;

    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        uint32_t rip = ((uint32_t)(rand() & 0xFFFF) << 16) | (rand() & 0xFFFF);
        char s_rip[20];
        f_ip(rip, s_rip);

        if (chk(rip, gw, mk)) {
            printf("Packet %d: %s -> LOCAL\n", i + 1, s_rip);
            loc++;
        } else {
            printf("Packet %d: %s -> REMOTE\n", i + 1, s_rip);
        }
    }

    printf("\nStats:\nLocal: %d (%.1f%%)\nRemote: %d (%.1f%%)\n", 
           loc, (float)loc / n * 100, 
           n - loc, (float)(n - loc) / n * 100);

    return 0;
}
