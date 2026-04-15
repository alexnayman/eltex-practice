#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    char *fname = argv[1];
    key_t key = ftok(fname, 1);
    int semid = semget(key, 1, 0666);
    if (semid == -1) return 1;

    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {0, 1, SEM_UNDO};

    char line[1024];

    while (1) {
        semop(semid, &lock, 1);

        FILE *f = fopen(fname, "r+");
        if (f) {
            int has_data = 0;
            while (fgets(line, sizeof(line), f)) {
                has_data = 1;
                int min, max, val, n, offset = 0;
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
                    printf("PID %d | File: %s | Max: %d, Min: %d\n", getpid(), fname, max, min);
                }
            }

            if (has_data) {
                freopen(fname, "w", f);
            }
            fclose(f);
        }

        semop(semid, &unlock, 1);
        sleep(2);
    }

    return 0;
}
