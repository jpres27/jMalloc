CFLAGS=-Wall -Wextra -std=gnu11 -pedantic

jallocator: jmalloc.c
	$(CC) $(CFLAGS) -o jallocator jmalloc.c