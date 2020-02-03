#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>


/*
**
*/
#include "my_list.h"

/*
**
*/

// get stat for file.
struct stat *get_stat(char *filename) {
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
  return file_info->st_mtime;
}

/* convert struct time_t to struct tm ==> gives access to local time */
struct tm *format_time(time_t time) {
  struct tm *formatted = malloc(sizeof(struct tm));
  localtime_r(&time, formatted); /* method from lib time.h */
  return formatted;
}

void            print_list(t_list *head, t_opts *opts) {
  t_list        *current_node = head;

  while (current_node != NULL) {
    char *filename = ((char *) current_node->data); // cast the data to a string.
    printf("%s\n", filename);
    current_node = current_node->next;
  }
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

t_list          *create_list() {
  t_list        *new_list = malloc(sizeof(t_list));
  new_list->data = NULL;
  new_list->next = NULL;
  return new_list;
}

/*
**
*/

t_list          *set_is_hidden(t_list *head, char *filename) {
  if (filename[0] == '.') {
    head->is_hidden = 1;
  }
  return head;
}

t_list          *create_name_only_list(char *filename) {
  t_list *head = create_list();
  head = set_is_hidden(head, filename);
  void *p;
  p = filename;
  head->data = p;
  return head;
}

t_list          *create_file_list(DIR *dirp, char *filename) {
  t_list *head = create_list();
  head = set_is_hidden(head, filename);
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
  return head;
}


// testing.
int main(int argc, char **argv) {
  char              *filename = argv[1];
  struct stat       *file_info = get_stat(filename);
  t_list            *file_list;
  DIR               *dirp = opendir(filename);

  if (S_ISREG(file_info->st_mode)) {
    file_list = create_name_only_list(filename);
  } else if (S_ISDIR(file_info->st_mode)) {
    file_list = create_file_list(dirp, filename);
  }
  print_list(file_list);
  free(file_info);
  destroy_list(&file_list);

  return 0;
}

// works for when passed a directory!!
// TODO:
// 1. if not a directory, print filename ==> done, 1/31 (NOT A LIST).
// 2. add necessary stat info to the void pointer data for each list node.
// 3. SORT LIST FUNCTIONS - OPTIONS ==>
  // sort alphabetically ==> done, 1/30.
  // create a way to accept options.
  // -a : include files starting with a "."
  // -t : sort/display time modified.
  // -R : recursively list sub-directories.
  // -z : display number of bytes in each file.


  // struct stat   *file_stats = get_stat(filename);
  // if ((S_ISDIR(file_stats->st_mode))) {
  // } else {
  //   void *p;
  //   p = filename;
  //   file_list->data = p;
  // }
  // free(file_stats);
