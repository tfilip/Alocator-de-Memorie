CC=gcc
CFLAGS=-Wall 

build: main.o
	$(CC) $(CFLAGS) -o allocator

main.o: main.c
	$(CC) $(CFLAGS) -c

clean:
	rm -rf main.o allocator