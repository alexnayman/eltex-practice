#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    char tname[256];
    snprintf(tname, sizeof(tname), "%s.tmp.%d", name, getpid());

    while (1) {
        char buf[4096];
        int ok = 0;

        lock(s);
        FILE *f = fopen(name, "r");
        if (f) {
            if (fgets(buf, sizeof(buf), f)) {
                ok = 1;
                FILE *t = fopen(tname, "w");
                if (t) {
                    char tb[4096];
                    while (fgets(tb, sizeof(tb), f)) {
                        fputs(tb, t);
                    }
                    fclose(t);
                    rename(tname, name);
                }
            }
            fclose(f);
        }
        unlock(s);

        if (ok) {
            int min = -1, max = -1, first = 1;
            char *p = strtok(buf, " \n");
            
            while (p) {
                int v = atoi(p);
                if (first) {
                    min = v;
                    max = v;
                    first = 0;
                } else {
                    if (v < min) min = v;
                    if (v > max) max = v;
                }
                p = strtok(NULL, " \n");
            }
            
            if (!first) {
                printf("[%s] Min: %d | Max: %d\n", name, min, max);
            }
        } else {
            sleep(1);
        }
    }
    
    return 0;
}
