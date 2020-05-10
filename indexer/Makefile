# Variables
L = ../libcs50
M = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I $M -I $L
LLIBS = $M/common.a $L/libcs50-given.a 

all: indexer indextest

indexer: indexer.o
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

indextest: indextest.o
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

# Phony Targets
.PHONY: clean test valgrind

clean:
	rm -f core *core.*
	rm -f indexer indextest *~ *.o

test: 
	bash -v testing.sh

valgrind:
	valgrind ./indexer ../crawler/output/letters-depth-6 ./indexout
	valgrind ./indextest ./indexout ./new
	rm -f indexout new


