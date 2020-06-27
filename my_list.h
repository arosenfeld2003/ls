/*
**
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

/*
**
*/

#ifndef NULL
  #define NULL (void *)0
#endif

/*
**
*/

typedef struct List {
  char            *filename;
  char            *full_pathname;
  struct stat     *info;
  int             is_dir;
  struct List     *next;
} t_list;

typedef struct Options {
  int include_hidden_files; // -a
  int show_size_in_bytes; // -z
  int list_dirs_recursively; // -R
  int sort_by_time_modified; // -t
} t_opts;

/*
**
*/
char            **set_filenames(int argc, char **args);
t_opts          *pass_in_options(int argc,  char **args, t_opts *opts);
void            destroy_list(t_list **head);
t_list          *create_list();
t_list          *create_new_node(t_list *head, char *filename, char *full_pathname);
t_list          *read_directory(t_list *head /* t_opts *opts */);
t_list          *delete_nodes(t_list *curr, char value);
t_list          *append_to_list(t_list *current_list, t_list *new_node);

void            print_list(t_list *head, t_opts *opts);

t_opts          *init_opts();
t_opts          get_opts(char *flags, t_opts *opts);
t_opts          *create_opts(char **args, int i);
t_list          *sort_with_options(t_list *file_list, t_opts *opts);

off_t           get_size(struct stat *file_info);
time_t          get_mod_time(struct stat *file_info);
struct          tm *format_time(time_t time);

void            split_node(t_list *source, t_list **front, t_list** back);
t_list          *alpha_sorted_merge(t_list *a, t_list *b);
void            merge_sort(t_list **headRef, t_opts *opts);
int             compare_size(long long a, long long b);
int             compare_strings(char *a, char*b);

char            *get_full_pathname(char *path, char *filename);
bool            can_recurse_dir(char* curr);
void            recurse(t_list *file_list, t_opts *opts);
void            my_ls(int argc, char **args);

