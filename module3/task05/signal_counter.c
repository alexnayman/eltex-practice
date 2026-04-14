#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define LOG_FILE "counter.log"
#define MAX_SIGNALS 3

volatile sig_atomic_t sig_count = 0;
volatile sig_atomic_t running = 1;
volatile sig_atomic_t last_signal = 0;

void handler(int sig) {
    if (sig == SIGINT) {
        sig_count++;
        last_signal = SIGINT;
        if (sig_count >= MAX_SIGNALS) {
            running = 0;
        }
    } else if (sig == SIGQUIT) {
        last_signal = SIGQUIT;
    }
}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    printf("PID: %d\n", getpid());

    int counter = 0;
    while (running) {
        counter++;
        
        FILE *f = fopen(LOG_FILE, "a");
        if (f) {
            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            char buf[64];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
            
            fprintf(f, "[%s] Counter: %d\n", buf, counter);

            if (last_signal == SIGINT) {
                fprintf(f, "--- Received SIGINT (Total: %d) ---\n", sig_count);
                last_signal = 0;
            } else if (last_signal == SIGQUIT) {
                fprintf(f, "--- Received SIGQUIT ---\n");
                last_signal = 0;
            }

            fclose(f);
        }

        printf("Counter: %d\n", counter);
        sleep(1);
    }

    return 0;
}
