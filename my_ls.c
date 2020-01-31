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
  return file_info->st_mtime;
}

/* convert struct time_t to struct tm ==> gives access to local time */
struct tm *format_time(time_t time) {
  struct tm *formatted = malloc(sizeof(struct tm));
  localtime_r(&time, formatted); /* method from lib time.h */
  return formatted;
}

void          print_list(t_list *head) {
  t_list *current_node = head;
  while (current_node != NULL) {
    char *filename = ((char *) current_node->data); // cast the data to a string.
    printf("%s\n", filename);
    current_node = current_node->next;
  }
}

// test & experiments
int main() {

  t_list *new_list = create_list();

  void *p; // void pointer variable.
  char *a = "Alex"; // a variable contains char 'a'.
  p = a; // assign value of a varibale to void pointer.
  new_list->data = p; // assign the data to the list.
  t_list *new_node = create_list();
  void *p2; // void pointer variable.
  char *b = "Bob"; // a variable contains char 'a'.
  p2 = b; // assign value of a varibale to void pointer.
  new_node->data = p2;
  new_list = append_to_list(new_list, new_node);
  print_list(new_list);

  // char test = *((char *) new_list->data); // cast the data to a char.
  // printf("%c\n", test);
  // t_list *new_node = create_list();
  // char b = 'B';
  // void *p2;
  // p2 = &b;
  // new_node->data = p2;
  // new_list = append_to_list(new_list, new_node);
  // char test2 = *((char *) new_list->next->data); // cast the data to a char.
  // printf("%c\n", test2);


  // t_list *prepend = create_list();
  // char c = 'C';
  // void *p3;
  // p3 = &c;
  // prepend->data = p3;
  // prepend_to_list(&new_list, prepend);

  // char test3 = *((char *) new_list->data);
  // char test4 = *((char *) new_list->next->data);
  // printf("%c\n", test3);
  // printf("%c\n", test4);

  destroy_list(&new_list);
  return 0;
}


/*
void my_putc(char);

int main () {
  void(*F)(char c);
  F = my_putc;
  F('B');
  F('\n');

  return 0;
}

void my_putc(char c) {
  write(1, &c, 1);
}
*/


// /*
// **
// */
// #include "my_list.h"

// /*
// **
// */
// int       main(int ac, char **av){
//   t_list  *list;

//   destroy_list(&list);
// }