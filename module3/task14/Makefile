CC = gcc
CFLAGS = -Wall -Wextra
TARGET = sniffer
SOURCE = udp_sniffer.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	sudo ./$(TARGET) $(PORT)
