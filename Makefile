CC=gcc
CFLAGS=-Wall -Wextra -Werror -g

OBJ = main.o file_list.o helpers.o

all: my_ls

my_ls: $(OBJ)
	$(CC) $(CFLAGS) -o my_ls $(OBJ)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

file_list.o: file_list.c file_list.h helpers.c
	$(CC) $(CFLAGS) -c file_list.c -o file_list.o

helpers.o: helpers.c
	$(CC) $(CFLAGS) -c helpers.c -o helpers.o

clean:
	rm -f *.o my_ls

fclean: clean
	rm -f my_ls