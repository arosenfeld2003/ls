#ifndef FILE_LIST_H
#define FILE_LIST_H

#include <sys/types.h>
#include <stdlib.h>

typedef struct {
    char *name;
    time_t mod_time;
    long mod_nsec;
} file_entry;

// file linked-list node
typedef struct file_entry_node {
    file_entry* entry;
    struct file_entry_node* next;
} file_entry_node;

// helpers
void list_directory(const char *path, int opt_a, int opt_t);
int int str_len(char *str);
char* str_cpy(char* dst, char* src);
void parse_args(int argc, char *argv[], int *opt_a, int *opt_t);

// merge sort
static void merge_sort(file_entry **entries, int count, int (*comp)(const void *, cons void *));
static file_entry **merge(file_entry, **left, int left_count, file_entry **right, int right_count, int (*comp)(const void *, const void *));

#endif

#ifndef NULL
#define NULL (void *)0
#endif