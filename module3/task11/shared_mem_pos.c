#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#define SHM_NAME "/posix_shm_task"

struct shared_data {
    int count;
    int numbers[100];
    int min;
    int max;
    int state;
};

int total_sets = 0;
struct shared_data *data;

void handle_sigint(int sig) {
    (void)sig;
    printf("\nTotal sets processed: %d\n", total_sets);
    munmap(data, sizeof(struct shared_data));
    shm_unlink(SHM_NAME);
    exit(0);
}

void child_handle_sigint(int sig) {
    (void)sig;
    munmap(data, sizeof(struct shared_data));
    exit(0);
}

int main() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) return 1;

    ftruncate(fd, sizeof(struct shared_data));
    data = mmap(NULL, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

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
