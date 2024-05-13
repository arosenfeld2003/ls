#ifndef FILE_LIST_H
#define FILE_LIST_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

// check for nanoseconds - only since POSIX 2008
#ifdef __USE_XOPEN2K8
// Use st_mtim for systems where it is supported
#define MOD_TIME_SEC(entry) ((entry).st_mtim.tv_sec)
#define MOD_TIME_NSEC(entry) ((entry).st_mtim.tv_nsec)
#else
// Fallback to st_mtime for systems without st_mtim, treating nanoseconds as 0
#define MOD_TIME_SEC(entry) ((entry).st_mtime)
#define MOD_TIME_NSEC(entry) (0)
#endif

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

// general helpers
void list_directory(const char *path, int opt_a, int opt_t);
int str_len(const char *str);
char* str_cpy(char* dst, char* src);
int parse_args(int argc, char *argv[], int *opt_a, int *opt_t, char **path);
// void free_entries(file_entry **entries, int count);

// merge sort helpers
void merge_sort(file_entry_node **headRef, int(*comp)(const void*, const void*));
void **merge(file_entry_node **left, int left_count, file_entry **right, int right_count, int (*comp)(const void*, const void*));
int compare_by_mod_time(const void *a, const void *b);
int compare_by_name(const void *a, const void *b);
//file_entry_node *add_to_list(file_entry_node *head, file_entry *new_entry);

#endif

#ifndef NULL
#define NULL (void *)0
#endif

