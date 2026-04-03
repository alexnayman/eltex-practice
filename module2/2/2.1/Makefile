CC = gcc
TARGET = phonebook
OBJS = phonebook_main.o phonebook_funcs.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

phonebook_main.o: phonebook_main.c phonebook_funcs.h
	$(CC) $(CFLAGS) -c phonebook_main.c

phonebook_funcs.o: phonebook_funcs.c phonebook_funcs.h
	$(CC) $(CFLAGS) -c phonebook_funcs.c

clean:
	rm -f $(OBJS) $(TARGET)