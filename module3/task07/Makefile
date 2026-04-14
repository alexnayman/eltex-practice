CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lrt

all: chat_a chat_b

chat_a: chat_a.c
	$(CC) $(CFLAGS) -o chat_a chat_a.c $(LDFLAGS)

chat_b: chat_b.c
	$(CC) $(CFLAGS) -o chat_b chat_b.c $(LDFLAGS)

clean:
	$(RM) chat_a chat_b *.o

.PHONY: all clean
