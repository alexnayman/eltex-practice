all: phonebook

phonebook: phonebook_main.o libphone.a
	gcc phonebook_main.o -L. -lphone -o phonebook

libphone.a: phonebook_funcs.o
	ar rcs libphone.a phonebook_funcs.o

phonebook_main.o: phonebook_main.c phonebook_funcs.h
	gcc -c phonebook_main.c

phonebook_funcs.o: phonebook_funcs.c phonebook_funcs.h
	gcc -c phonebook_funcs.c

clean:
	rm -f *.o phonebook libphone.a
