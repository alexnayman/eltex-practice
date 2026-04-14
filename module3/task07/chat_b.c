#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/select.h>

#define MQ_A "/mq_a"
#define MQ_B "/mq_b"
#define MSG_SIZE 256
#define STOP_PRIO 10

int main(void) {
    mqd_t mq_a = mq_open(MQ_A, O_RDWR);
    mqd_t mq_b = mq_open(MQ_B, O_RDWR);

    if (mq_a < 0 || mq_b < 0) {
        perror("mq_open (Start Chat A first!)");
        return 1;
    }

    printf("Chat B: Connected. Type a message or 'quit'.\n");

    char buf[MSG_SIZE];
    unsigned int prio;

    while (1) {
        fd_set rfds;
        struct timeval tv;
        int mq_fd = mq_b;

        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        FD_SET(mq_fd, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 100000;

        int ret = select(mq_fd + 1, &rfds, NULL, NULL, &tv);
        if (ret < 0) break;

        if (FD_ISSET(mq_fd, &rfds)) {
            ssize_t n = mq_receive(mq_b, buf, MSG_SIZE, &prio);
            if (n > 0) {
                buf[n] = '\0';
                if (prio == STOP_PRIO) {
                    printf("\n[A stopped]: Chat ending.\n");
                    mq_send(mq_a, "STOP", 5, STOP_PRIO);
                    break;
                }
                printf("\n[A]: %s\n[B]> ", buf);
                fflush(stdout);
            }
        }

        if (FD_ISSET(STDIN_FILENO, &rfds)) {
            if (fgets(buf, MSG_SIZE, stdin)) {
                buf[strcspn(buf, "\n")] = '\0';
                if (strlen(buf) == 0) continue;

                if (strcmp(buf, "quit") == 0) {
                    printf("Sending STOP to A. Bye.\n");
                    mq_send(mq_a, "STOP", 5, STOP_PRIO);
                    break;
                }
                mq_send(mq_a, buf, strlen(buf) + 1, 1);
            } else {
                break;
            }
        }
    }

    mq_close(mq_a);
    mq_close(mq_b);
    return 0;
}
