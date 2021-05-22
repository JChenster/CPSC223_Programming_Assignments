CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: testOrderBook

testOrderBook: testOrderBook.o orderBook.o
	$(CC) $(CFLAGS) -o $@ $^

testOrderBook.o: testOrderBook.c orderBook.h
orderBook.o: orderBook.c orderBook.h

clean:
	$(RM) *.o