all: a1 a2file a2

a1: a1.c a1main.c a1.h
	gcc -g -Wall -pedantic -ansi a1main.c a1.c -o a1
a2file: a1 post.cc
	./a1 post.cc
a2: post.c addauthor.c stream.h libstream.a
	gcc -g -Wall -ansi post.c -o post.o -c
	gcc -g -Wall -ansi addauthor.c -o addauthor.o -c
	gcc post.o -o post -lstream -L.
	gcc addauthor.o -o addauthor -lstream -L.
libstream.a:
	gcc -Wall -g -ansi stream.c -o stream.o -c
	ar cr libstream.a stream.o
clean:
	rm -f post a1 a.out post.c
	rm -f addauthor
	rm -f -r post.dSYM
	rm -f -r a1.dSYM
	rm -f sample1.c sample2.c sample3.c
	rm -f -r addauthor.dSYM
	rm -f stream.o
	rm -f post.o
	rm -f libstream.a
	rm -f addauthor.o
removemessages:
	rm -r messages
	mkdir messages
