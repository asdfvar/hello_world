all: foo.o libfoo.so main.o
	gcc -o test main.o -L. -lfoo

clean:
	rm -fv foo.o main.o libfoo.so test

foo.o:
	gcc -c -fpic foo.c

libfoo.so:
	gcc -shared -o libfoo.so foo.o

main.o:
	gcc -c main.c
