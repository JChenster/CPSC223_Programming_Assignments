CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: pfml

pfml: pfml.o flip.o
	$(CC) $(CFLAGS) -o $@ $^

pfml.o: pfml.c flip.h

flip.o: flip.c flip.h

clean:
	$(RM) *.o

test:
	./pfml