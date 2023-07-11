# Makefile for common library
#   Builds and compiles all common utility functions into a library.
#
# Christian Nyamekye, Spring 2023.

# object files, and the target library
OBJS = ../libcs50/libcs50-given.a webpage.o ../common/pagedir.o ../common/index.o ../common/words.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

.PHONY: all clean

# Build $(LIB) by archiving object files
# (you will need to drop in copy of set.c, counters.c, hashtable.c)
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h 
	$(CC) $(CFLAGS) -c ../libcs50/webpage.h ../libcs50/file.h ../libcs50/mem.h

words.o: words.h

index.o: index.h ../libcs50/hashtable.h

webpage.o:  ../libcs50/webpage.c ../libcs50/webpage.h
	$(CC) $(CFLAGS) -c ../libcs50/webpage.c

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
