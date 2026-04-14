#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

union sem_u {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int get_sem(const char *name) {
    int fd = open(name, O_CREAT | O_RDWR, 0666);
    if (fd >= 0) close(fd);

    key_t k = ftok(name, 1);
    int s = semget(k, 1, IPC_CREAT | IPC_EXCL | 0666);
    
    if (s >= 0) {
        union sem_u u;
        u.val = 1;
        semctl(s, 0, SETVAL, u);
    } else {
        s = semget(k, 1, 0666);
    }
    
    return s;
}

void lock(int s) {
    struct sembuf b = {0, -1, 0};
    semop(s, &b, 1);
}

void unlock(int s) {
    struct sembuf b = {0, 1, 0};
    semop(s, &b, 1);
}

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    
    char *name = argv[1];
    int s = get_sem(name);
    srand(time(NULL) ^ getpid());

    while (1) {
        int n = rand() % 10 + 5;
        
        lock(s);
        FILE *f = fopen(name, "a");
        if (f) {
            for (int i = 0; i < n; i++) {
                fprintf(f, "%d ", rand() % 1000);
            }
            fprintf(f, "\n");
            fclose(f);
        }
        unlock(s);
        
        sleep(1);
    }
    
    return 0;
}
