CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
OBJS = indexer.o  ../common/pagedir.o ../common/index.o ../common/words.o ../libcs50/libcs50-given.a 
LIBS =

# for memory leaks
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all clean

all: indexer indextest

indexer: indexer.o ../common/pagedir.o ../common/index.o ../common/words.o ../libcs50/libcs50-given.a 
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o ../common/index.o ../libcs50/libcs50-given.a
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: indexer.c ../common/pagedir.h ../common/index.h ../common/words.h ../libcs50/libcs50-given.a
	$(CC) $(CFLAGS) -c indexer.c 

indextest.o: indextest.c ../common/pagedir.h ../common/index.h ../libcs50/libcs50-given.a
	$(CC) $(CFLAGS) -c indextest.c 

index.o: index.c index.h
	$(CC) $(CFLAGS) -c index.c

words.o: words.c words.h
	$(CC) $(CFLAGS) -c words.c

webpage.o: ../libcs50/webpage.c ../libcs50/webpage.h
	$(CC) $(CFLAGS) -c ../libcs50/webpage.c

hashtable.o: ../libcs50/hashtable.c ../libcs50/hashtable.h
	$(CC) $(CFLAGS) -c ../libcs50/hashtable.c

bag.o: ../libcs50/bag.c ../libcs50/bag.h
	$(CC) $(CFLAGS) -c ../libcs50/bag.c

test: indexer indextest testing.sh
	bash -v testing.sh >& testing.out
	./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../data/toscrape.index
	./indextest ../data/toscrape.index ../data/toscrape.index_copy
	~/cs50-dev/shared/tse/indexcmp cmp1 cmp2 

# using valgrind to check for leaks.
valgrind: indexer
	$(VALGRIND) ./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../data/toscrape.index
	$(VALGRIND) ./indextest ../data/toscrape.index ../data/toscrape.index_copy

.PHONY: clean sourcelist

clean:
	rm -f *~ *.o *.dSYM
	rm -f indexer
	rm -f core
	rm -f indextest
