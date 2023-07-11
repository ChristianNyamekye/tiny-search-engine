CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
OBJS = crawler.o ../common/pagedir.o ../libcs50/libcs50.a
LIBS =

# for memory leaks
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all


crawler: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

crawler.o: crawler.h crawler.c
	$(CC) $(CFLAGS) -c crawler.c ../libcs50/webpage.h ../common/pagedir.h

webpage.o: ../libcs50/webpage.c ../libcs50/webpage.h
	$(CC) $(CFLAGS) -c ../libcs50/webpage.c

hashtable.o: ../libcs50/hashtable.c ../libcs50/hashtable.h
	$(CC) $(CFLAGS) -c ../libcs50/hashtable.c

bag.o: ../libcs50/bag.c ../libcs50/bag.h
	$(CC) $(CFLAGS) -c ../libcs50/bag.c

test: testing.sh
	bash -v testing.sh >& testing.out

# using valgrind to check for leaks.
valgrind: crawler
	./valgrind.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f crawler
	rm -f core
	rm -f $(LIB) *~ *.o