# Makefile for 'hashtable' module
# I copied the 'bag' module makefile and edited so it fits the 'hashtable' module
# CS 50, Fall 2022
# Aya Hajjeh

OBJS = hashtabletest.o hashtable.o hash.o ../lib/file.o ../lib/mem.o 
LIBS =

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

hashtabletest: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

hashtabletest.o: hashtable.h ../lib/file.h
hashtable.o: hashtable.h
hash.o: hash.h
../lib/file.o: ../lib/file.h

.PHONY: test valgrind clean

# expects a file `test.names` to exist; it can contain any text.
test: hashtabletest test.names
	./hashtabletest < test.names

# expects a file `test.names` to exist; it can contain any text.
valgrind: hashtabletest test.names
	$(VALGRIND) ./hashtabletest < test.names

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f hashtabletest
	rm -f core
