#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define MSG_KEY 0x1234
#define MAX_TEXT 256

typedef struct {
    long mtype;
    int sender_id;
    char text[MAX_TEXT];
} Message;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: client <client_id>\n");
        return 1;
    }

    int my_id = atoi(argv[1]);
    long my_type = 20 + my_id;

    int msqid = msgget(MSG_KEY, 0666);
    if (msqid < 0) {
        perror("msgget");
        return 1;
    }

    Message srv;
    srv.mtype = 10;
    srv.sender_id = my_id;
    snprintf(srv.text, MAX_TEXT, "connect");
    msgsnd(msqid, &srv, sizeof(srv) - sizeof(long), 0);

    pid_t pid = fork();

    if (pid == 0) {
        Message incoming;
        while (1) {
            if (msgrcv(msqid, &incoming, sizeof(incoming) - sizeof(long), my_type, 0) > 0) {
                printf("\n[Client %d]: %s\nYou> ", incoming.sender_id, incoming.text);
                fflush(stdout);
            }
        }
    } else {
        char buf[MAX_TEXT];
        Message out;
        out.mtype = 10;
        out.sender_id = my_id;

        while (1) {
            printf("You> ");
            fflush(stdout);
            if (!fgets(buf, MAX_TEXT, stdin)) break;
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) == 0) continue;

            strncpy(out.text, buf, MAX_TEXT - 1);
            out.text[MAX_TEXT - 1] = '\0';
            msgsnd(msqid, &out, sizeof(out) - sizeof(long), 0);

            if (strcmp(buf, "shutdown") == 0) break;
        }

        kill(pid, SIGTERM);
    }

    return 0;
}
