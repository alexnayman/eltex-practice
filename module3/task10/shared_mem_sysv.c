#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define SHM_SIZE 1024

struct shared_data {
    int count;
    int numbers[100];
    int min;
    int max;
    int state; 
};

int shmid;
struct shared_data *data;
int total_sets = 0;

void handle_sigint(int sig) {
    (void)sig;
    printf("\nTotal sets processed: %d\n", total_sets);
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}

void child_handle_sigint(int sig) {
    (void)sig;
    shmdt(data);
    exit(0);
}

int main() {
    shmid = shmget(IPC_PRIVATE, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shmid == -1) return 1;

    data = shmat(shmid, NULL, 0);
    data->state = 0;

    signal(SIGINT, handle_sigint);

    pid_t pid = fork();

    if (pid == 0) {
        signal(SIGINT, child_handle_sigint);
        
        while (1) {
            if (data->state == 1) {
                int min = data->numbers[0];
                int max = data->numbers[0];
                
                for (int i = 1; i < data->count; i++) {
                    if (data->numbers[i] < min) min = data->numbers[i];
                    if (data->numbers[i] > max) max = data->numbers[i];
                }
                
                data->min = min;
                data->max = max;
                data->state = 2;
            }
            usleep(1000);
        }
    } else {
        srand(time(NULL));
        
        while (1) {
            if (data->state == 0) {
                data->count = rand() % 10 + 2;
                for (int i = 0; i < data->count; i++) {
                    data->numbers[i] = rand() % 100;
                }
                data->state = 1;
            } else if (data->state == 2) {
                printf("Set %d | Min: %d, Max: %d\n", total_sets + 1, data->min, data->max);
                total_sets++;
                data->state = 0;
                sleep(1);
            }
            usleep(1000);
        }
    }

    return 0;
}
