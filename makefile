all: a1 a2file a2 a3 sql folders

a1: A2/a1.c A2/a1main.c A2/a1.h
	gcc -g -Wall -pedantic -ansi A2/a1main.c A2/a1.c -o a1
a2file: a1 A2/post.cc
	./a1 A2/post.cc
a2: A2/post.c A2/addauthor.c A2/stream.h libstream.a
	gcc -g -Wall -ansi A2/post.c -o post.o -c
	gcc -g -Wall -ansi A2/addauthor.c -o addauthor.o -c
	gcc post.o -o post -lstream -L. -lmysqlclient -L/usr/lib/x86_64-linux-gnu/
	gcc addauthor.o -o addauthor -lstream -L. -lmysqlclient -L/usr/lib/x86_64-linux-gnu/
a3: parser.c a1 a2
	gcc -Wall -pedantic -ansi -g parser.c -o Parser
libstream.a:
	gcc -Wall -g -ansi A2/stream.c -o stream.o -c 
	ar cr libstream.a stream.o
sql: db.c
	gcc -Wall -g -ansi db.c -o db -lmysqlclient -L/usr/lib/x86_64-linux-gnu/
folders:
	mkdir -p messages
	mkdir -p bin
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
	rm -f Parser
	rm -r -f Parser.dSYM
	rm -r -f a.out.dSYM
	rm -f db
removemessages:
	rm -f -r messages
	mkdir messages
regeneratephp:
	rm -f RemoveAuthorOutput.php
	rm -f RemoveAuthor.php
	rm -f AddAuthorOutput.php
	rm -f AddAuthor.php
	rm -f ChooseAStream.php
	rm -f Post.php
	rm -f PostOutput.php
clobber: clean removemessages regeneratephp
