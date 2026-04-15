#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define Q1 "/queue_a_to_b"
#define Q2 "/queue_b_to_a"
#define EXIT_PRIO 255
#define MAX_SIZE 256

int main() {
    mqd_t q1, q2;
    struct mq_attr attr;
    char buf[MAX_SIZE];
    unsigned int p;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    q1 = mq_open(Q1, O_CREAT | O_WRONLY, 0666, &attr);
    q2 = mq_open(Q2, O_CREAT | O_RDONLY, 0666, &attr);

    if (q1 == (mqd_t)-1 || q2 == (mqd_t)-1) return 1;

    while (1) {
        printf("Chat A: ");
        if (!fgets(buf, MAX_SIZE, stdin)) break;
        buf[strcspn(buf, "\n")] = 0;

        if (strcmp(buf, "exit") == 0) {
            mq_send(q1, buf, MAX_SIZE, EXIT_PRIO);
            break;
        }

        mq_send(q1, buf, MAX_SIZE, 1);

        if (mq_receive(q2, buf, MAX_SIZE, &p) == -1) break;
        if (p == EXIT_PRIO) break;
        
        printf("Chat B: %s\n", buf);
    }

    mq_close(q1);
    mq_close(q2);
    mq_unlink(Q1);
    mq_unlink(Q2);

    return 0;
}
