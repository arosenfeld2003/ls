CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
DEPS = my_list.h
OBJFILES = build_file_list.o main.o parse_args.o \
						print.o sort.o destroy.o options.c
TARGET = my_ls

all: $(TARGET)

$(TARGET): $(OBJFILES) $(DEPS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

.PHONY: clean
clean:
	rm -f $(OBJFILES) $(TARGET) *~