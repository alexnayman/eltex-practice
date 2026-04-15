#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t last_signal = 0;
volatile sig_atomic_t sigint_count = 0;

void h(int s) {
    last_signal = s;
    if (s == SIGINT) {
        sigint_count++;
    }
}

int main(void) {
    int c = 1;

    FILE* f = fopen("out.txt", "w");
    if (!f) return 1;

    struct sigaction sa;
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    while (1) {
        if (last_signal != 0) {
            if (last_signal == SIGINT || last_signal == SIGQUIT) {
                fprintf(f, "sig %d\n", (int)last_signal);
                fflush(f);
            }

            if (sigint_count >= 3) {
                break;
            }

            last_signal = 0;
        }

        fprintf(f, "%d\n", c++);
        fflush(f);

        sleep(1);
    }

    fclose(f);
    return 0;
}
