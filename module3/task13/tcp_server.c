#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

void error(const char *msg) { perror(msg); exit(1); }

void dostuff(int sock) {
    char buff[1024];
    float a, b, res;
    char op;
    int n;

    while (1) {
        const char *menu = "\n--- MENU ---\n1. Calculator\n2. File Transfer\n3. Quit\n> ";
        if (write(sock, menu, strlen(menu)) < 0) return;

        bzero(buff, sizeof(buff));
        n = read(sock, buff, sizeof(buff) - 1);
        if (n <= 0) return;

        if (buff[0] == '1') {
            write(sock, "Enter 1st number: ", 18);
            bzero(buff, sizeof(buff));
            read(sock, buff, sizeof(buff) - 1);
            a = atof(buff);

            write(sock, "Enter operation (+, -, *, /): ", 30);
            bzero(buff, sizeof(buff));
            read(sock, buff, sizeof(buff) - 1);
            op = (buff[0] == '\n' || buff[0] == '\r') ? buff[1] : buff[0];

            write(sock, "Enter 2nd number: ", 18);
            bzero(buff, sizeof(buff));
            read(sock, buff, sizeof(buff) - 1);
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
            if (fd < 0) {
                write(sock, "FS Error\n", 9);
                continue;
            }
            while (1) {
                bzero(buff, sizeof(buff));
                n = read(sock, buff, sizeof(buff));
                if (n <= 0) break;
                if (strstr(buff, "EOF_MARKER")) {
                    char *p = strstr(buff, "EOF_MARKER");
                    write(fd, buff, p - buff);
                    break;
                }
                write(fd, buff, n);
            }
            close(fd);
            write(sock, "Status: Success\n", 16);
        }
        else if (buff[0] == '3') break;
    }
    close(sock);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) error("ERROR: No port");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR: Socket failed");

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        perror("setsockopt failed");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR: Binding failed (Port busy?)");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("SERVER RUNNING ON PORT %d\n", portno);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) continue;

        printf("+ Connection: %s\n", inet_ntoa(cli_addr.sin_addr));

        if (fork() == 0) {
            close(sockfd);
            dostuff(newsockfd);
            printf("- Finished: %s\n", inet_ntoa(cli_addr.sin_addr));
            exit(0);
        }
        close(newsockfd);
    }
    return 0;
}
