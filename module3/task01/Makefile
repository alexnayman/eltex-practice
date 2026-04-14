CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = task01

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	$(RM) $(TARGET) *.o

.PHONY: all clean
