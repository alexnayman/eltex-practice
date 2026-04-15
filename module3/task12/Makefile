CC = gcc
CFLAGS = -Wall -Wextra

all: client_a client_b

client_a: udp_client_a.c
	$(CC) $(CFLAGS) udp_client_a.c -o client_a

client_b: udp_client_b.c
	$(CC) $(CFLAGS) udp_client_b.c -o client_b

clean:
	rm -f client_a client_b
