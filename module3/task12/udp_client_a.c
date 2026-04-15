#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define BUF_SIZE 1024
#define MY_PORT 8001
#define PEER_PORT 8002

int main() {
    int sockfd;
    struct sockaddr_in my_addr, peer_addr;
    char buf[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) return 1;

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MY_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) return 2;

    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(PEER_PORT);
    peer_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    pid_t pid = fork();

    if (pid == 0) {
        while (1) {
            int n = recvfrom(sockfd, buf, BUF_SIZE, 0, NULL, NULL);
            if (n > 0) {
                buf[n] = '\0';
                printf("\nB: %s\n> ", buf);
                fflush(stdout);
            }
        }
    } else {
        while (1) {
            printf("> ");
            if (!fgets(buf, BUF_SIZE, stdin)) break;
            buf[strcspn(buf, "\n")] = 0;
            sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&peer_addr, sizeof(peer_addr));
            if (strcmp(buf, "exit") == 0) {
                kill(pid, SIGKILL);
                break;
            }
        }
    }

    close(sockfd);
    return 0;
}
