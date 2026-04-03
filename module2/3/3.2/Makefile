CC = gcc
OBJ = net_main.o net_func.o
TGT = net_sim

all: $(TGT)

$(TGT): $(OBJ)
	$(CC) $(OBJ) -o $(TGT)

%.o: %.c net.h
	$(CC) -c $<

clean:
	rm -f *.o $(TGT)
