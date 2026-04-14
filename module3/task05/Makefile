CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = signal_counter

all: $(TARGET)

$(TARGET): signal_counter.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(TARGET) counter.log *.o

.PHONY: all clean
