CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lrt

all: shared_mem_pos

shared_mem_pos: shared_mem_pos.c
	$(CC) $(CFLAGS) shared_mem_pos.c -o shared_mem_pos $(LIBS)

clean:
	rm -f shared_mem_pos
