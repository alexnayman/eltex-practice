CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = phonebook3
OBJS = main.o phonebook_io.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c phonebook.h
	$(CC) $(CFLAGS) -c main.c

phonebook_io.o: phonebook_io.c phonebook.h
	$(CC) $(CFLAGS) -c phonebook_io.c

clean:
	$(RM) $(TARGET) *.o *.txt

.PHONY: all clean
