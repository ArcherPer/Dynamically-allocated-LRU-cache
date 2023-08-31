CC = cc

install:
	$(CC) -Wall -Wpedantic cache.c test.c -o test