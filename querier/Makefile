# Makefile for 'querier' module
#
# Christian Nyamekye, Spring 2023

L = ../libcs50

PROG = querier
OBJS = querier.o 
LIBS = 
LLIBS = $L/libcs50.a ../common/common.a

FUZZPROG = fuzzquery
FUZZOBJS = fuzzquery.o
FUZZLIBS = 
FUZZLLIBS = $L/libcs50.a ../common/common.a


# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST


CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I../common
MAKE = make

.PHONY: all clean

all: $(PROG) $(FUZZPROG)


$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(^) $(LLIBS) $(LIBS) -o $(@)

querier.o: $L/bag.h $L/hashtable.h $L/webpage.h ../common/pagedir.h ../common/index.h

$(FUZZPROG): $(FUZZOBJS) $(FUZZLLIBS)
	$(CC) $(CFLAGS) $(^) $(FUZZLLIBS) $(FUZZLIBS) -o $(@)

fuzzquery.o: $L/file.h $L/mem.h

test:
	# ./querier ~/cs50-dev/shared/tse/output/toscrape-1 ~/cs50-dev/shared/tse/indices/toscrape-1.ndx
	bash -v testing.sh >& testing.out
	
valgrind: $(PROG)
	valgrind ./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 10 11111 | ./querier ~/cs50-dev/shared/tse/crawldata/letters-2 ~/cs50-dev/shared/tse/output/toscrape-2.index
	
.PHONY: clean test

clean:
	rm -f *~ *.o *.dSMY
	rm -f $(PROG) $(FUZZPROG)
	rm -f core

	
