CC=gcc
CFLAGS=-Wall 

build: alocator.o
	$(CC) $(CFLAGS) $^ -o alocator

main.o: alocator.c
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -rf main.o alocator
