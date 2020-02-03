/*
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
  void          *data;
  int           is_hidden;
  void          *file_size;
  void          *mod_time;
  int           is_dir;
  struct List   *next;
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

void            destroy_list(t_list **head);
t_list          *create_list();
t_list          *create_file_list(DIR *dirp, char *filename);
t_list          *create_name_only_list(char *filename);
t_list          *delete_nodes(t_list *curr, char value);
t_list          *append_to_list(t_list *current_list, t_list *new_node);
void            split_node(t_list *source, t_list **front, t_list** back);
t_list          *alpha_sorted_merge(t_list *a, t_list *b);
void            merge_sort(t_list **headRef, t_opts *opts);
void            print_list(t_list *head);
void            destroy_list(t_list **head);

// void         apply_on_list(t_list *my_list, int(*func)(void *));

t_opts          *init_opts();

struct          stat *get_info(char *filename);
off_t           get_size(struct stat *file_info);
time_t          get_mod_time(struct stat *file_info);
struct          tm *format_time(time_t time);

int             compare_size(long long a, long long b);
int             compare_strings(char *a, char*b);

