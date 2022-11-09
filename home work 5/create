CC = gcc
CFLAGS = -Wall -g

6.45e: 6.45e.c 6.42.o
	$(CC) $(CFLAGS) 6.45e.c 6.42.o -o $@

6.42.o: 6.42.c queue42.h
	$(CC) $(CFLAGS) -c $< -o $@
