#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#define MSG_KEY 0x1234
#define MAX_TEXT 256

typedef struct {
    long mtype;
    int sender_id;
    char text[MAX_TEXT];
} Message;

int msqid;
volatile int running = 1;

void handler(int sig) {
    (void)sig;
    running = 0;
}

int main(void) {
    msqid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msqid < 0) {
        perror("msgget");
        return 1;
    }

    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    printf("Server started. Waiting for messages...\n");

    Message msg;
    int clients[64];
    int nclients = 0;

    while (running) {
        if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 10, IPC_NOWAIT) > 0) {
            int sender = msg.sender_id;

            int found = 0;
            for (int i = 0; i < nclients; i++) {
                if (clients[i] == sender) { found = 1; break; }
            }
            if (!found && nclients < 64) {
                clients[nclients++] = sender;
            }

            if (strcmp(msg.text, "shutdown") == 0) {
                for (int i = 0; i < nclients; i++) {
                    if (clients[i] == sender) {
                        for (int j = i; j < nclients - 1; j++)
                            clients[j] = clients[j + 1];
                        nclients--;
                        break;
                    }
                }
                printf("Client %d disconnected\n", sender);
                continue;
            }

            printf("Client %d: %s\n", sender, msg.text);

            for (int i = 0; i < nclients; i++) {
                if (clients[i] != sender) {
                    Message out = msg;
                    out.mtype = 20 + clients[i]; 
                    msgsnd(msqid, &out, sizeof(out) - sizeof(long), 0);
                }
            }
        } else {
            usleep(100000);
        }
    }

    msgctl(msqid, IPC_RMID, NULL);
    printf("Server stopped\n");
    return 0;
}
