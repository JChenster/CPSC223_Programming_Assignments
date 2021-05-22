CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: coloring

coloring: color.o graph.o
	$(CC) $(CFLAGS) -o $@ $^

color.o: color.c graph.h

graph.o: graph.c graph.h

clean:
	$(RM) *.o coloring

