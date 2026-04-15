#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>

#define MAX_CLIENTS 32
#define BUF_SIZE 1024

void handle_client(int sock) {
    char buff[BUF_SIZE];
    float a, b, res;
    char op;
    int n;

    bzero(buff, sizeof(buff));
    n = read(sock, buff, sizeof(buff) - 1);
    
    if (n <= 0) {
        printf("- Client disconnected\n");
        close(sock);
        return;
    }

    if (buff[0] == '1') {
        write(sock, "Enter 1st number: ", 18);
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        a = atof(buff);

        write(sock, "Enter operation: ", 17);
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        op = (buff[0] == '\n') ? buff[1] : buff[0];

        write(sock, "Enter 2nd number: ", 18);
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        b = atof(buff);

        if (op == '+') res = a + b;
        else if (op == '-') res = a - b;
        else if (op == '*') res = a * b;
        else if (op == '/') res = (b != 0) ? a / b : 0;
        else res = 0;

        snprintf(buff, sizeof(buff), "Result: %.3f\n", res);
        write(sock, buff, strlen(buff));
    } 
    else if (buff[0] == '2') {
        write(sock, "READY_FOR_FILE", 14);
        int fd = open("uploaded.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
        while (1) {
            bzero(buff, sizeof(buff));
            n = read(sock, buff, sizeof(buff));
            if (n <= 0 || strstr(buff, "EOF_MARKER")) break;
            write(fd, buff, n);
        }
        close(fd);
        write(sock, "Status: Success\n", 16);
    }
    
    if (buff[0] != '3') {
        const char *menu = "\n--- MENU ---\n1. Calculator\n2. File Transfer\n3. Quit\n> ";
        write(sock, menu, strlen(menu));
    } else {
        close(sock);
    }
}

int main(int argc, char *argv[]) {
    int listen_sock, conn_sock, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    struct pollfd fds[MAX_CLIENTS + 1];

    if (argc < 2) { fprintf(stderr, "Usage: %s <port>\n", argv[0]); exit(1); }

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(listen_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(listen_sock, 5);

    for (int i = 0; i < MAX_CLIENTS + 1; i++) fds[i].fd = -1;

    fds[0].fd = listen_sock;
    fds[0].events = POLLIN;

    printf("MULTIPLEXED SERVER RUNNING ON PORT %s\n", argv[1]);

    while (1) {
        int ret = poll(fds, MAX_CLIENTS + 1, -1);
        if (ret < 0) perror("poll error");

        if (fds[0].revents & POLLIN) {
            conn_sock = accept(listen_sock, (struct sockaddr *)&cli_addr, &clilen);
            printf("+ New connection: %s\n", inet_ntoa(cli_addr.sin_addr));
            
            const char *menu = "\n--- MENU ---\n1. Calculator\n2. File Transfer\n3. Quit\n> ";
            write(conn_sock, menu, strlen(menu));

            for (int i = 1; i < MAX_CLIENTS + 1; i++) {
                if (fds[i].fd < 0) {
                    fds[i].fd = conn_sock;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }

        for (int i = 1; i < MAX_CLIENTS + 1; i++) {
            if (fds[i].fd > 0 && (fds[i].revents & POLLIN)) {
                handle_client(fds[i].fd);
                struct sockaddr_in tmp;
                socklen_t len = sizeof(tmp);
                if (getpeername(fds[i].fd, (struct sockaddr*)&tmp, &len) < 0) {
                    fds[i].fd = -1;
                }
            }
        }
    }
    return 0;
}
