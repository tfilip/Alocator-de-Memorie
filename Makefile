CC=gcc
CFLAGS=-Wall 

build: main.o
	$(CC) $(CFLAGS) main.o -o allocator

main.o: main.c
	$(CC) $(CFLAGS) main.c -c

clean:
	rm -rf main.o allocator