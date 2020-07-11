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

#ifndef NULL
  #define NULL (void *)0
#endif

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

/* user must free filenames array */
char **set_filenames(int argc, char **argv) {
  char **filenames = NULL;
  char *file = NULL;
  int file_count = 0;

  if ((argc == 1) || (argc == 2 && argv[1][0] == '-')) {
    file_count = 1;
    file = ".";
  } else {
    int i = 1;
    // skip the flags.
    while (argv[i][0] == '-') {
      i++;
    }
    // count files.
    while (i < argc) {
      file_count++;
      i++;
    }
  }
  filenames = malloc(sizeof(char *) * file_count + 1);
  if (file != NULL) {
    filenames[0] = malloc(sizeof(char) * strlen(file) + 1);
    strcpy(filenames[0], file);
  } else {
    int i = 0;
    int j;
    switch(argv[1][0]) {
      case '-':
        j = 2;
        break;
      default:
        j = 1;
    }
    while (j < argc) {
      filenames[i] = malloc(sizeof(char) * strlen(argv[j]) + 1);
      strcpy(filenames[i], argv[j]);
      i++;
      j++;
    }
    filenames[i] = NULL;
  }
  return filenames;
}

/*
  This works on Linux filesystems - foward slashes separate directories.
*/
char *get_full_pathname(char *path, char *filename) {
  /* handle filenames of the current and parent directories */
  if (strcmp(path, filename) == 0) {
    return filename;
  }
  char *fullpath = malloc(strlen(filename) + strlen(path) + 2);

  /* handle case of parent directory ==> " ../ " */
  if (strcmp(path, "../") == 0) {
    sprintf(fullpath, "%s%s", path, filename);
  } else {
    sprintf(fullpath, "%s/%s", path, filename);
  }
  return fullpath;
}

/* create an empty node */
t_list *create_node() {
  t_list *new_node = malloc(sizeof(t_list));
  new_node->filename = NULL;
  new_node->full_pathname = NULL;
  new_node->info = malloc(sizeof(struct stat));
  new_node->is_dir = 0;
  new_node->next = NULL;
  return new_node;
}

t_list *make_user_filelist(int argc, char **argv, t_list *file_list) {
  struct stat *buf_info = malloc(sizeof(struct stat));
  char **filenames = set_filenames(argc, argv);
  t_list *head = file_list;
  int i = 0;
  while (filenames[i] != NULL) {
    /* Filename and full pathname are the same for user args */
    file_list->filename = filenames[i];
    file_list->full_pathname = filenames[i];
    stat(file_list->full_pathname, buf_info);
    if (S_ISDIR(buf_info->st_mode)) {
      file_list->is_dir = 1;
    }
    file_list->info = buf_info;
    file_list->next = create_node();
    file_list = file_list->next;
    i++;
  }
  return head;
}

#include "my_list.h"

/* user must free filenames array */
char **set_filenames(int argc, char **argv) {
  char **filenames = NULL;
  char *file = NULL;
  int file_count = 0;

  if ((argc == 1) || (argc == 2 && argv[1][0] == '-')) {
    file_count = 1;
    file = ".";
  } else {
    int i = 1;
    // skip the flags.
    while (argv[i][0] == '-') {
      i++;
    }
    // count files.
    while (i < argc) {
      file_count++;
      i++;
    }
  }
  filenames = malloc(sizeof(char *) * file_count + 1);
  if (file != NULL) {
    filenames[0] = malloc(sizeof(char) * strlen(file) + 1);
    strcpy(filenames[0], file);
  } else {
    int i = 0;
    int j;
    switch(argv[1][0]) {
      case '-':
        j = 2;
        break;
      default:
        j = 1;
    }
    while (j < argc) {
      filenames[i] = malloc(sizeof(char) * strlen(argv[j]) + 1);
      strcpy(filenames[i], argv[j]);
      i++;
      j++;
    }
    filenames[i] = NULL;
  }
  return filenames;
}

/*
  This works on Linux filesystems - foward slashes separate directories.
*/
char *get_full_pathname(char *path, char *filename) {
  /* handle filenames of the current and parent directories */
  if (strcmp(path, filename) == 0) {
    return filename;
  }
  char *fullpath = malloc(strlen(filename) + strlen(path) + 2);
  /* handle case of parent directory ==> " ../ " */
  if (strcmp(path, "../") == 0) {
    sprintf(fullpath, "%s%s", path, filename);
  } else {
    sprintf(fullpath, "%s/%s", path, filename);
  }
  return fullpath;
}

// initialize with no flags.
t_opts *init_opts() {
  t_opts *opts = malloc(sizeof(t_opts));
  opts->include_hidden_files = 0; // -a
  opts->show_size_in_bytes = 0; // -z
  opts->list_dirs_recursively = 0; // -R
  opts->sort_by_time_modified = 0; // -t
  return opts;
}

char *create_flags(int argc, char **argv) {
  char *flags = NULL;
  int flags_len = 0;
  int flag_count = 0;

  // count number of flags and allocate space.
  int i = 1;
  while (i < argc) {
    if (argv[i][0] ==  '-') {
      flag_count += (strlen(argv[i]) - 1);
    }
    i++;
  }
  flags = malloc(sizeof(char) * flag_count + 1);
  char temp_head[flags_len];
  char *temp_ptr = temp_head;
  int len, j;
  i = 1;

  while (i < argc) {
    j = 0;
    if (argv[i][j] == '-') {
      j++;
      len = strlen(argv[i]);
      while (j < len) {
        *temp_ptr = argv[i][j];
        temp_ptr++;
        j++;
      }
    }
    i++;
  }
  *temp_ptr = '\0';
  strcpy(flags, temp_head);
  return flags;
}

t_opts *get_opts(char *flags, t_opts *opts) {
  int i = 0;
  while (flags[i]) {
    switch(flags[i]) {
      case 'a':
        opts->include_hidden_files = 1;
        break;
      case 'z':
        opts->show_size_in_bytes = 1;
        break;
      case 'R':
        opts->list_dirs_recursively = 1;
        break;
      case 't':
        opts->sort_by_time_modified = 1;
        break;
      default:
        printf("option %c invalid\n", flags[i]);
    }
    i++;
  }
  return opts;
}

t_opts *create_opts(int argc, char **argv) {
  char *flags = create_flags(argc, argv);
  t_opts *opts = init_opts();
  return get_opts(flags, opts);
}

#include "./my_list.h"

/*
  Split nodes of given list into front and back halves, and return the two lists.
  If source length is odd, the extra node goes in the front list.
  Use "fast/slow" pointer strategy.
*/
void split_node(t_list *source, t_list **front, t_list** back) {
  t_list *slow = source;
  t_list *fast = source->next;
  /* Advance 'fast' two nodes, and advance 'slow' one node */
  while (fast != NULL) {
    fast = fast->next;
    if (fast != NULL) {
      slow = slow->next;
      fast = fast->next;
    }
  }
  /* 'slow' is before the midpoint in the list - split at that point. */
  *front = source;
  *back= slow->next;
  slow->next = NULL;
}


/* Merge two sorted lists into one list, in alphabetical order based on filename. */
t_list *alpha_sorted_merge(t_list *a, t_list *b) {
  t_list *result = NULL;
  /* Base cases */
  if (a == NULL)
    return (b);
  else if (b == NULL)
    return (a);
  /* Pick either a or b, and recur */
  if (compare_strings(a->filename, b->filename) == 1 ||
    compare_strings(a->filename, b->filename) == 0) {
      result = b;
      result->next = alpha_sorted_merge(a, b->next);
    } else {
    result = a;
    result->next = alpha_sorted_merge(a->next, b);
  }
  return (result);
}

/* merge lists in sorted order based on time modified */
t_list *time_mod_sorted_merge(t_list *a, t_list *b) {
  t_list *result = NULL;
  /* Base cases */
  if (a == NULL) {
    return (b);
  } else if (b == NULL) {
    return (a);
  }
  if (compare_size((long long)a->info->st_mtime, (long long)b->info->st_mtime) == 1 ||
        compare_size((long long)a->info->st_mtime, (long long)b->info->st_mtime) == 0) {
    result = a;
    result->next = time_mod_sorted_merge(b, a->next);
  } else {
    result = b;
    result->next = time_mod_sorted_merge(b->next, a);
  }
  return (result);
}


/* merge lists in sorted order based on time modified */
t_list *file_size_sorted_merge(t_list *a, t_list *b) {
  t_list *result = NULL;
  /* Base cases */
  if (a == NULL) {
    return (b);
  } else if (b == NULL) {
    return (a);
  }
  if (compare_size((long long)a->info->st_size, (long long)b->info->st_size) == 1 ||
        compare_size((long long)a->info->st_size, (long long)b->info->st_size) == 0) {
    result = a;
    result->next = file_size_sorted_merge(b, a->next);
  } else {
    result = b;
    result->next = file_size_sorted_merge(b->next, a);
  }
  return (result);
}

void merge_sort(t_list **headRef, t_opts *opts) {
  t_list *head = *headRef;
  t_list *a;
  t_list *b;
  /* Base case -- length 0 */
  if ((head == NULL) || (head->next == NULL)) {
    return;
  }
  /* Split head into 'a' and 'b' sublists */
  split_node(head, &a, &b);
  /* Recursively break down the sublists */
  merge_sort(&a, opts);
  merge_sort(&b, opts);
  if (opts && opts->sort_by_time_modified == 1) {
    *headRef = time_mod_sorted_merge(a, b);
  } else {
    /* merge lists sorted alphabetically */
    *headRef = alpha_sorted_merge(a, b);
  }
}

t_list *sort_with_options(t_list *file_list, t_opts *opts) {
  /* handle -a option */
  if (opts->include_hidden_files == 0) {
    /* if no flag, remove all files starting with a dot from list. */
    char dot = '.';
    file_list = delete_nodes(file_list, dot);
  }
  /* handle other options */
  merge_sort(&file_list, opts);
  return file_list;
}

void my_ls(int argc, char **argv) {
  
}

int main(int argc, char **argv) {
  t_list *new_node = create_node();
  t_list *file_list = make_user_filelist(argc, argv, new_node);
  t_opts *opts = create_opts(argc, argv);



  return 0;
}