#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define Q_KEY 1234
#define SERVER_TYPE 10
#define MAX_TEXT 256
#define MAX_CLIENTS 100

struct msg_buf {
    long mtype;
    int sender;
    char text[MAX_TEXT];
};

int main() {
    int qid, cl[MAX_CLIENTS], n = 0;
    struct msg_buf m;

    qid = msgget(Q_KEY, IPC_CREAT | 0666);
    if (qid == -1) {
        perror("msgget");
        return 1;
    }

    printf("Server started. Queue ID: %d\n", qid);
    printf("Waiting for messages...\n\n");

    while (1) {
        if (msgrcv(qid, &m, sizeof(m) - sizeof(long), SERVER_TYPE, 0) == -1) {
            perror("msgrcv");
            break;
        }

        int exist = 0;
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (cl[i] == m.sender) {
                exist = 1;
                idx = i;
                break;
            }
        }

        if (!exist && n < MAX_CLIENTS) {
            cl[n++] = m.sender;
            idx = n - 1;
            printf("[NEW CONNECTION] Client %d joined. Total clients: %d\n", m.sender, n);
        }

        if (strcmp(m.text, "end") == 0) {
            printf("[SHUTDOWN] Client %d left the chat.\n", m.sender);
            if (idx != -1) {
                for (int i = idx; i < n - 1; i++) cl[i] = cl[i+1];
                n--;
            }
            printf("Active clients remaining: %d\n", n);
            continue;
        }

        printf("[MSG] From %d: %s\n", m.sender, m.text);

        int s = m.sender;
        for (int i = 0; i < n; i++) {
            if (cl[i] != s) {
                m.mtype = cl[i];
                if (msgsnd(qid, &m, sizeof(m) - sizeof(long), 0) == -1) {
                    perror("msgsnd");
                }
            }
        }
        fflush(stdout);
    }

    return 0;
}
