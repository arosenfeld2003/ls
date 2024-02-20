CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: my_ls

my_ls: main.o file_list.o
	$(CC) $(CFLAGS) -o my_ls main.o file_list.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

file_list.o: file_list.c file_list.h
	$(CC) $(CFLAGS) -c file_list.c

clean:
	rm -f *.o my_ls