#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

FILE* f;
volatile sig_atomic_t sc = 0;

void h(int s) {
    if (s == SIGINT || s == SIGQUIT) {
        fprintf(f, "sig %d\n", s);
        fflush(f);
    }
    
    if (s == SIGINT) {
        sc++;
        if (sc == 3) {
            fclose(f);
            exit(0);
        }
    }
}

int main(void) {
    int c = 1;
    
    f = fopen("out.txt", "w");
    if (!f) return 1;

    struct sigaction sa;
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    while (1) {
        fprintf(f, "%d\n", c++);
        fflush(f);
        sleep(1);
    }

    return 0;
}
