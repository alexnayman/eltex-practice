CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L
LIBS = -lrt

all: chat_a chat_b

chat_a: chat_a.c
	$(CC) $(CFLAGS) chat_a.c -o chat_a $(LIBS)

chat_b: chat_b.c
	$(CC) $(CFLAGS) chat_b.c -o chat_b $(LIBS)

clean:
	rm -f chat_a chat_b
