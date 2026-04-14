CC = gcc
CFLAGS = -Wall -Wextra

all: producer consumer

producer: producer.c
	$(CC) $(CFLAGS) producer.c -o producer

consumer: consumer.c
	$(CC) $(CFLAGS) consumer.c -o consumer

clean:
	rm -f producer consumer *.tmp.*
