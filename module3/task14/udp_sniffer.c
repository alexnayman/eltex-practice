#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <unistd.h>

void process_packet(unsigned char* buffer, int size) {
    struct iphdr *ip = (struct iphdr *)buffer;
    
    unsigned short iphdrlen = ip->ihl * 4;
    struct udphdr *udp = (struct udphdr *)(buffer + iphdrlen);

    int s_port = ntohs(udp->source);
    int d_port = ntohs(udp->dest);

    if (s_port == 8001 || s_port == 8002 || d_port == 8001 || d_port == 8002) {
        printf("\n[%s:%d] ---> ", inet_ntoa(*(struct in_addr *)&ip->saddr), s_port);
        printf("[%s:%d]\n", inet_ntoa(*(struct in_addr *)&ip->daddr), d_port);

        unsigned char *data = buffer + iphdrlen + sizeof(struct udphdr);
        int payload_size = ntohs(udp->len) - sizeof(struct udphdr);

        printf("  TEXT: ");
        for (int i = 0; i < payload_size; i++) {
            printf("%c", (data[i] >= 32 && data[i] <= 126) ? data[i] : '.');
        }

        printf("\n  HEX : ");
        for (int i = 0; i < payload_size; i++) {
            printf("%02X ", data[i]);
        }
        printf("\n  -------------------------------------------");
    }
}

int main() {
    int raw_sock;
    unsigned char buffer[65536];

    raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_sock < 0) {
        perror("Run as root!");
        return 1;
    }

    printf("Sniffer is running... (Waiting for chat messages)\n");

    while (1) {
        int size = recvfrom(raw_sock, buffer, 65536, 0, NULL, NULL);
        if (size > 0) process_packet(buffer, size);
    }
    return 0;
}
