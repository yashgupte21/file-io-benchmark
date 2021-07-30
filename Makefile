CC=gcc
CFLAGS=-Wall
OFLAGS=-O3
PTHREAD=-lpthread

all: fileio run-fileio

fileio: fileio.c
	$(CC) fileio.c -o fileio ${PTHREAD} $(CFLAGS) $(OFLAGS)

run-fileio: run-fileio.sh
	./run-fileio.sh

clean:
	rm fileio
	rm fileio_bash
