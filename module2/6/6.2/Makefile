all: phonebook

phonebook: phonebook_main.o libphone.so
	gcc phonebook_main.o -L. -lphone -Wl,-rpath,. -o phonebook

libphone.so: phonebook_funcs.o
	gcc -shared -o libphone.so phonebook_funcs.o

phonebook_main.o: phonebook_main.c phonebook_funcs.h
	gcc -c phonebook_main.c

phonebook_funcs.o: phonebook_funcs.c phonebook_funcs.h
	gcc -c -fPIC phonebook_funcs.c

clean:
	rm -f *.o phonebook libphone.so
