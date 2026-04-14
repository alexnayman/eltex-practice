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
    struct mq_attr attr = { .mq_flags = 0, .mq_maxmsg = 10, .mq_msgsize = MSG_SIZE };

    mq_unlink(MQ_A);
    mq_unlink(MQ_B);

    mqd_t mq_a = mq_open(MQ_A, O_CREAT | O_RDWR, 0666, &attr);
    mqd_t mq_b = mq_open(MQ_B, O_CREAT | O_RDWR, 0666, &attr);

    if (mq_a < 0 || mq_b < 0) {
        perror("mq_open");
        return 1;
    }

    printf("Chat A: Queues ready. Type a message or 'quit'.\n");

    char buf[MSG_SIZE];
    unsigned int prio;

    while (1) {
        fd_set rfds;
        struct timeval tv;
        int mq_fd = mq_a;

        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        FD_SET(mq_fd, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 100000;

        int ret = select(mq_fd + 1, &rfds, NULL, NULL, &tv);
        if (ret < 0) break;

        if (FD_ISSET(mq_fd, &rfds)) {
            ssize_t n = mq_receive(mq_a, buf, MSG_SIZE, &prio);
            if (n > 0) {
                buf[n] = '\0';
                if (prio == STOP_PRIO) {
                    printf("\n[B stopped]: Chat ending.\n");
                    mq_send(mq_b, "STOP", 5, STOP_PRIO);
                    break;
                }
                printf("\n[B]: %s\n[A]> ", buf);
                fflush(stdout);
            }
        }

        if (FD_ISSET(STDIN_FILENO, &rfds)) {
            if (fgets(buf, MSG_SIZE, stdin)) {
                buf[strcspn(buf, "\n")] = '\0';
                if (strlen(buf) == 0) continue;

                if (strcmp(buf, "quit") == 0) {
                    printf("Sending STOP to B. Bye.\n");
                    mq_send(mq_b, "STOP", 5, STOP_PRIO);
                    break;
                }
                mq_send(mq_b, buf, strlen(buf) + 1, 1);
            } else {
                break;
            }
        }
    }

    mq_close(mq_a);
    mq_close(mq_b);
    mq_unlink(MQ_A);
    mq_unlink(MQ_B);
    return 0;
}
