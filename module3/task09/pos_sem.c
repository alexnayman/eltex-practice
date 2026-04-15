#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

#define SEM_NAME "/posix_file_sem"
#define FILE_NAME "data.txt"

sem_t *sem;

void handle_sigint(int sig) {
    (void)sig;
    sem_close(sem);
    sem_unlink(SEM_NAME);
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);

    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        char line[1024];
        while (1) {
            sem_wait(sem);

            FILE *f = fopen(FILE_NAME, "r+");
            if (f) {
                int has_data = 0;
                while (fgets(line, sizeof(line), f)) {
                    has_data = 1;
                    int min, max, val, n;
                    int first = 1;
                    char *ptr = line;

                    while (sscanf(ptr, "%d%n", &val, &n) == 1) {
                        if (first) {
                            min = max = val;
                            first = 0;
                        } else {
                            if (val < min) min = val;
                            if (val > max) max = val;
                        }
                        ptr += n;
                    }
                    if (!first) {
                        printf("Child (PID %d) | Max: %d, Min: %d\n", getpid(), max, min);
                    }
                }
                
                if (has_data) {
                    freopen(FILE_NAME, "w", f);
                }
                fclose(f);
            }

            sem_post(sem);
            sleep(2);
        }
    } else {
        srand(time(NULL));
        while (1) {
            sem_wait(sem);

            FILE *f = fopen(FILE_NAME, "a");
            if (f) {
                int count = rand() % 10 + 1;
                for (int i = 0; i < count; i++) {
                    fprintf(f, "%d ", rand() % 100);
                }
                fprintf(f, "\n");
                fclose(f);
            }

            sem_post(sem);
            sleep(1);
        }
    }

    return 0;
}
