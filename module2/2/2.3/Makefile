CC = gcc
TARGET = calc
OBJS = calc_main.o calc_funcs.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

calc_main.o: calc_main.c calc_funcs.h
	$(CC) $(CFLAGS) -c calc_main.c

calc_funcs.o: calc_funcs.c calc_funcs.h
	$(CC) $(CFLAGS) -c calc_funcs.c

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe