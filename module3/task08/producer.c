#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    char *fname = argv[1];
    key_t key = ftok(fname, 1);
    int semid = semget(key, 1, IPC_CREAT | 0666);

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;

    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {0, 1, SEM_UNDO};

    srand(time(NULL));

    while (1) {
        semop(semid, &lock, 1);

        FILE *f = fopen(fname, "a");
        if (f) {
            int count = rand() % 10 + 1;
            for (int i = 0; i < count; i++) {
                fprintf(f, "%d ", rand() % 100);
            }
            fprintf(f, "\n");
            fclose(f);
        }

        semop(semid, &unlock, 1);
        sleep(1);
    }

    return 0;
}
