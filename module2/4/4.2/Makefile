CC = gcc

all: queue_prog

queue_prog: queue_main.o priority_queue.o
	$(CC) -o queue_prog queue_main.o priority_queue.o

queue_main.o: queue_main.c priority_queue.h
	$(CC) -c queue_main.c

priority_queue.o: priority_queue.c priority_queue.h
	$(CC) -c priority_queue.c

clean:
	rm -f *.o queue_prog
