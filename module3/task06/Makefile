CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGETS = server client

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) server.c -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f $(TARGETS)
	ipcrm -Q 1234 2>/dev/null || true
