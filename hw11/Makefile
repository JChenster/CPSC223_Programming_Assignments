CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: testInserter

testInserter: testInserter.o inserter.o
	$(CC) $(CFLAGS) -o $@ $^

testInserter.o: testInserter.c inserter.h

inserter.o: inserter.c inserter.h

clean:
	$(RM) *.o

