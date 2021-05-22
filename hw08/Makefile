CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: compress

compress: compress.o compressionDict.o helperFunctions.o
	$(CC) $(CFLAGS) -o $@ $^

compress.o: compress.c compressionDict.h helperFunctions.h compressionFormat.h

compressionDict.o: compressionDict.c compressionDict.h helperFunctions.h compressionFormat.h

helperFunctions.o: helperFunctions.c helperFunctions.h compressionFormat.h

clean:
	$(RM) *.o

test:
	./compress
