#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

#ifndef NULL
  #define NULL (void *)0
#endif

typedef struct List {
  char            *filename;
  char            *path;
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


t_list      *create_node();
t_list      *make_user_filelist(int argc, char **argv);
int         count_files(int argc, char **argv);
char        **set_filenames(int argc, char **argv);
t_opts      *init_opts();
char        *create_flags(int argc, char **argv);
t_opts      *get_opts(char *flags, t_opts *opts);
t_opts      *create_opts(int argc, char **argv);
void        split_node(t_list *source,
              t_list **front, t_list** back);
t_list      *alpha_sorted_merge(t_list *a, t_list *b);
t_list      *time_mod_sorted_merge(t_list *a, t_list *b);
t_list      *file_size_sorted_merge(t_list *a, t_list *b);
void        merge_sort(t_list **headRef, t_opts *opts);
t_list      *skip_hidden_files(t_list *file_list, t_opts *opts);
t_list      *sort_with_options(t_list *file_list, t_opts *opts);
char        *formatdate(char *str, time_t val);
void        print_list(t_list *head, t_opts *opts);
void        print_directories(t_list *file_list, t_opts *opts);
void        free_node(t_list *node);
t_list      *delete_first_node(t_list *head);
t_list      *delete_node(t_list *file_list, char *filename);
void        destroy_list(t_list **head);