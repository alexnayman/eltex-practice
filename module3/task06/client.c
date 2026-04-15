#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define Q_KEY 1234
#define SERVER_TYPE 10
#define MAX_TEXT 256

struct msg_buf {
    long mtype;
    int sender;
    char text[MAX_TEXT];
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <client_id>\n", argv[0]);
        return 1;
    }

    int id = atoi(argv[1]);
    int qid = msgget(Q_KEY, 0666);
    if (qid == -1) {
        perror("msgget (server might not be running)");
        return 1;
    }

    struct msg_buf m;
    pid_t p = fork();

    if (p < 0) {
        perror("fork");
        return 1;
    }

    if (p == 0) {
        while (1) {
            if (msgrcv(qid, &m, sizeof(m) - sizeof(long), id, 0) > 0) {
                printf("\n[%d]: %s\n> ", m.sender, m.text);
                fflush(stdout);
            }
        }
    } else {
        
        m.mtype = SERVER_TYPE;
        m.sender = id;
        memset(m.text, 0, MAX_TEXT);
        msgsnd(qid, &m, sizeof(m) - sizeof(long), 0);

        while (1) {
            printf("> ");
            if (!fgets(m.text, MAX_TEXT, stdin)) break;
            m.text[strcspn(m.text, "\n")] = 0;
            
            if (strlen(m.text) == 0) continue;

            m.mtype = SERVER_TYPE;
            m.sender = id;
            msgsnd(qid, &m, sizeof(m) - sizeof(long), 0);

            if (strcmp(m.text, "end") == 0) {
                kill(p, SIGKILL);
                exit(0);
            }
        }
    }

    return 0;
}
