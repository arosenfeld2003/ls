#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "my_list.h"

t_list          *create_list() {
  t_list        *new_list = malloc(sizeof(t_list));
  new_list->data = NULL;
  new_list->next = NULL;
  return new_list;
}


void            destroy_list(t_list **head) {
  t_list        *current;
  t_list        *next;

  current = *head;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  head = NULL;
}


t_list          *append_to_list(t_list *current_list, t_list *new_node) {
  t_list        *head = current_list; // pointer to head of list.

  if (current_list->data == NULL) {
    errno = -1;
  } else {
    while (current_list->next != NULL) {
      current_list = current_list->next;
    }
    current_list->next = new_node;
  }

  current_list = head; // reset pointer to start of list.
  return head;
}



void         prepend_to_list(t_list **head, t_list *new_node) {
  if (new_node->data == NULL) {
    errno = -1;
  } else {
    new_node->next = *head;
    *head = new_node;
  }
}




// declare data type for function parameters?
void          apply_on_list(t_list *my_list, int(*func)(void *)) {
  t_list      *head = my_list;

  while (my_list->next != NULL) {
    func(my_list->data);
    my_list = my_list->next;
  }
  func(my_list->data);
  my_list = head;
}



struct stat *get_info(char *filename) {
  struct stat *buf = malloc(sizeof(stat)); /* malloc a buffer for the stat. */
  stat(filename, buf); /* fill stat buffer with stat of node. */
  return buf;
}

/* total size, in bytes */
off_t get_size(struct stat *file_info) {
  return file_info->st_size;
}

/* time of last modification */
time_t get_mod_time(struct stat *file_info) {
  time_t mod_time = file_info->st_mtime;
  return mod_time;
}

/* convert struct time_t to struct tm ==> gives access to local time */
struct tm *format_time(time_t time) {
  struct tm *formatted = malloc(sizeof(struct tm));
  localtime_r(&time, formatted); /* method from lib time.h */
  return formatted;
}


void         print_mod_time(t_list *head) {
  time_t mod_time = *((time_t *) head->mod_time);
  printf("%ld", mod_time);
}

void          print_list(t_list *head) {
  t_list *current_node = head;
  while (current_node != NULL) {
    char *filename = ((char *) current_node->data); // cast the data to a string.
    printf("%s", filename);

    // if (head->mod_time != NULL) {
    //   time_t mod_time = *((time_t *) head->mod_time);
    // }

    // print_mod_time(head);


    printf("\n");
    current_node = current_node->next;
  }
}

// initialize without any options.
t_opts *init_opts() {
  t_opts *opts = malloc(sizeof(t_opts));
  opts->include_hidden_files = 0; // -a
  opts->show_size_in_bytes = 0; // -z
  opts->list_dirs_recursively = 0; // -R
  opts->sort_by_time_modified = 0; // -t
  return opts;
}

t_opts get_opts(char *flags, t_opts *opts) {
  int i;
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
    }
    i++;
  }
  return *opts;
}

/* delete any nodes whose filenames start with char value */
t_list *delete_nodes(t_list *curr, char value) {
  t_list          *next;
  next = curr->next;

  /* See if we are at end of list. */
  if (curr->data == NULL) {
    return NULL;
  }

  /* see if the next node is empty */
  if (next == NULL) {
    return curr;
  }

  /* Check to see if current node is one to be deleted. */
  char *filename = ((char *) curr->data);
  if (filename[0] == value) {

    /* Deallocate the node. */
    free(curr);

    /* recursive call: continue checking list after deleted element */
    return delete_nodes(next, value);
  }

  /* recursive call: continue checking list after retained element */
  curr->next = delete_nodes(curr->next, value);

  /*
    Return the pointer to where we were called from.
    Since we did not remove this node it will be the same.
  */
  return curr;
}

/* create list when passed a file that isn't a directory */
t_list          *create_name_only_list(char *filename) {
  t_list *head = create_list();
  void *p;
  p = filename;
  head->data = p;
  return head;
}

/* create list when passed a directory */
t_list          *create_file_list(DIR *dirp, char *filename) {
  t_list *head = create_list();
  struct dirent *file;

  // continually open next directory stream until reaching the end of file list.
  while ((file = readdir(dirp)) != NULL) {
    char *name = file->d_name;
    void *p;
    p = name;

    if (head->data == NULL) {
      // empty list.
      head->data = p;
    } else {
      t_list *new_node = create_list();
      new_node->data = p;
      head = append_to_list(head, new_node);
    }
  }
  closedir(dirp);
  free(file);
  return head;
}


/*
******  Compare Methods  ******
*/

/* use long long in order to compare file size off_t data type */
int         compare_size(long long a, long long b) {
  return a > b ? 1 : (a < b ? -1 : 0);
}

/* returns: 1 if a is bigger, -1 is b is bigger, 0 if equal */
int         compare_strings(char *a, char*b) {
  int i = 0;
  int comp = compare_size(a[i], b[i]);

  // handle strings that start with the same letter.
  while (comp == 0) {
    i++;
    if (a[i] && b[i]) {
      /* keep comparing size of next letter */
      comp = compare_size(a[i], b[i]);
    } else if (!a[i] && !b[i]) {
      /* strings are exactly equal */
      return 0;
    } else {
      if (a[i]) {
        /* string a is longer */
        comp = 1;
      } else {
        /* string b is longer */
        comp = 2;
      }
    }
  }
  return comp;
}

int           compare_file_size(struct stat *file1, struct stat *file2) {
  off_t file1_size = file1->st_size;
  off_t file2_size = file2->st_size;
  int comp = compare_size(file1_size, file2_size);
  return comp;
}

/*
******  Sort Methods  ******
*/

/*
  Split nodes of given list into front and back halves, and return the two lists.
  If source length is odd, the extra node goes in the front list.
  Use "fast/slow" pointer strategy.
*/
void          split_node(t_list *source, t_list **front, t_list** back) {
  t_list      *fast;
  t_list      *slow;

  slow = source;
  fast = source->next;

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

/*
  Merge two sorted lists into one list, in alphabetical order based on filename.
*/
t_list        *alpha_sorted_merge(t_list *a, t_list *b) {
  t_list      *result = NULL;

  /* Base cases */
  if (a == NULL) {
    return (b);
  }

  else if (b == NULL) {
    return (a);
  }

  /* cast data from void pointer to an int */
  char *filename_a = ((char *) a->data);
  char *filename_b = ((char *) b->data);

  /* Pick either a or b, and recur */
  if (compare_strings(filename_a, filename_b) == 1 ||
    compare_strings(filename_a, filename_b) == 0) {
      result = b;
      result->next = alpha_sorted_merge(a, b->next);
    } else {
    result = a;
    result->next = alpha_sorted_merge(a->next, b);
  }
  return (result);
}

/* merge lists in sorted order based on time modified */
t_list        *time_mod_sorted_merge(t_list *a, t_list *b) {
  t_list      *result = NULL;

  /* Base cases */
  if (a == NULL) {
    return (b);
  }

  else if (b == NULL) {
    return (a);
  }

  /* get time mod */
  char *filename_a = ((char *) a->data);
  struct stat *file_info_a = get_info(filename_a);
  time_t mod_time_a = get_mod_time(file_info_a);
  // printf("%ld\n", mod_time_a);
  void *p = ((void *)mod_time_a);
  a->mod_time = p;
  free(file_info_a);

  char *filename_b = ((char *) b->data);
  struct stat *file_info_b = get_info(filename_b);
  time_t mod_time_b = get_mod_time(file_info_b);
  /* ** */
  // void *p2 = ((void *)mod_time_b);
  // b->mod_time = p2;
  /* ** */
  free(file_info_b);

  /* Pick either a or b, and recur */
  if (compare_size(mod_time_a, mod_time_b) == 1 ||
    compare_size(mod_time_a, mod_time_b) == 0) {
      result = a;
      result->next = time_mod_sorted_merge(b, a->next);
    } else {
    result = b;
    result->next = time_mod_sorted_merge(b->next, a);
  }
  return (result);
}

t_list        *handle_options(t_list *file_list, t_opts *opts) {
  /* handle -a option */
  if (opts->include_hidden_files == 0) {
    /* if no flag, remove all files starting with a dot from list. */
    char dot = '.';
    file_list = delete_nodes(file_list, dot);
  }

  /* handle option t */
  if (opts->sort_by_time_modified == 1) {
    t_list          *head = file_list;
    while (head != NULL) {
      char *filename = ((char *) head->data);
      struct stat *file_info = get_info(filename);
      time_t mod_time = get_mod_time(file_info);
      void *p;
      p = &mod_time;
      head->mod_time = p;
      free(file_info);
      head = head->next;
    }
  }
  return file_list;
}

void          merge_sort(t_list **headRef, t_opts *opts) {
  t_list      *head = *headRef;
  t_list      *a;
  t_list      *b;

  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL)) {
    return;
  }

  /* Split head into 'a' and 'b' sublists */
  split_node(head, &a, &b);

  /* Recursively break down the sublists */
  merge_sort(&a, opts);
  merge_sort(&b, opts);

  if (opts->sort_by_time_modified == 1) {
    *headRef = time_mod_sorted_merge(a, b);
  } else {
    /* merge lists sorted alphabetically */
    *headRef = alpha_sorted_merge(a, b);
  }
}

// testing main
int main(int argc, char **argv) {
  // For now, only accepts flags and/or one file arg

  // check for flags and options
  char              *flags = argv[1];
  char              *filename;
  t_opts            *opts = init_opts();

  if (flags[0] == '-') {
    flags++;
    *opts = get_opts(flags, opts);
    filename = argv[2];
  } else {
    filename = argv[1];
  }

  struct stat       *file_info = get_info(filename);
  t_list            *file_list;
  DIR               *dirp = opendir(filename);

  if (S_ISREG(file_info->st_mode)) {
    file_list = create_name_only_list(filename);
  } else if (S_ISDIR(file_info->st_mode)) {
    file_list = create_file_list(dirp, filename);
  }

  file_list = handle_options(file_list, opts);

  merge_sort(&file_list, opts);

  print_list(file_list);

  free(file_info);

  destroy_list(&file_list);

  return 0;
}


