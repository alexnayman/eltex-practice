CC = gcc
TGT = perm_calc
OBJ = perm_main.o perm_func.o

all: $(TGT)

$(TGT): $(OBJ)
	$(CC) -o $(TGT) $(OBJ)

%.o: %.c perm_func.h
	$(CC) -c $<

clean:
	rm -f $(OBJ) $(TGT)
