# Variables
L = ../libcs50

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I $L
OBJS = pagedir.o index.o word.o
LIB = common.a
LLIBS = $L/libcs50-given.a
MAKE = make

# Rules
$(LIB): $(OBJS) $(LLIBS)
	ar cr $(LIB) $^

pagedir.o: pagedir.h
index.o: index.h
word.o: word.h

# Phony Targets
.PHONY: clean

clean:
	rm -f core
	rm -f $(LIB) *~ *.o
	