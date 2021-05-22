CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: decrypt

decrypt: decrypter.o decrypt.o getInputs.o
	$(CC) $(CFLAGS) -o $@ $^

decrypter.o: decrypter.c getInputs.h decrypt.h
decrypt.o: decrypt.c decrypt.h getInputs.h
getInputs.o: getInputs.c getInputs.h

clean:
	$(RM) *.o

test:
	./decrypt salt passwords.dict < turtles.encrypted