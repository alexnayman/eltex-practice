CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = shell_pipe

all: $(TARGET)

$(TARGET): shell_pipe.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(TARGET) *.o

.PHONY: all clean
