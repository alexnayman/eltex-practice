CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -pthread

all: pos_sem

pos_sem: pos_sem.c
	$(CC) $(CFLAGS) pos_sem.c -o pos_sem $(LIBS)

clean:
	rm -f pos_sem data.txt
