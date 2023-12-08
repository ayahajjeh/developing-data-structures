# Makefile for 'counters' module
# I copied the 'bag' module makefile and edited so it fits the 'counters' module
# CS 50, Fall 2022
# Aya Hajjeh

OBJS = counterstest.o counters.o ../lib/file.o ../lib/mem.o 
LIBS =

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

bagtest: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

counterstest.o: counters.h ../lib/file.h
counters.o: counters.h
../lib/file.o: ../lib/file.h

.PHONY: test valgrind clean

# expects a file `test.names` to exist; it can contain any text.
test: counterstest test.names
	./counterstest < test.names

# expects a file `test.names` to exist; it can contain any text.
valgrind: counterstest test.names
	$(VALGRIND) ./counterstest < test.names

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f counterstest
	rm -f core
