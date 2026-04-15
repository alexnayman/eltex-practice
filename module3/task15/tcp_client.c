#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

void error(const char *msg) { perror(msg); exit(0); }

int main(int argc, char *argv[]) {
    int my_sock, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buff[1024];

    if (argc < 3) error("Usage: host port");

    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(argv[1]);
    if (!server) error("No such host");

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connect failed");

    while (1) {
        bzero(buff, sizeof(buff));
        n = recv(my_sock, buff, sizeof(buff)-1, 0);
        if (n <= 0) break;
        buff[n] = 0;

        if (strstr(buff, "READY_FOR_FILE")) {
            char filename[128];
            printf("\nLocal file name to send: ");
            fflush(stdout);
            scanf("%s", filename);
            int fd = open(filename, O_RDONLY);
            if (fd >= 0) {
                while ((n = read(fd, buff, sizeof(buff))) > 0) send(my_sock, buff, n, 0);
                send(my_sock, "EOF_MARKER", 10, 0);
                close(fd);
            } else {
                send(my_sock, "EOF_MARKER", 10, 0);
                printf("File not found!\n");
            }
            continue;
        }

        printf("%s", buff);
        fflush(stdout);

        if (strchr(buff, ':') || strchr(buff, '>')) {
            bzero(buff, sizeof(buff));
            fgets(buff, sizeof(buff)-1, stdin);
            if (buff[0] == '\n') fgets(buff, sizeof(buff)-1, stdin);
            send(my_sock, buff, strlen(buff), 0);
            if (buff[0] == '3') break;
        }
    }
    close(my_sock);
    return 0;
}
