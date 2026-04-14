CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGETS = shell sum_args concat max_num

all: $(TARGETS)

shell: shell.c
	$(CC) $(CFLAGS) -o $@ $<

sum_args: sum_args.c
	$(CC) $(CFLAGS) -o $@ $<

concat: concat.c
	$(CC) $(CFLAGS) -o $@ $<

max_num: max_num.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(TARGETS) *.o

.PHONY: all clean
