/*
**
*/

#include "my_list.h"

/*
**
*/

void            destroy_list(t_list **head) {
  t_list        *current;
  t_list        *next;

  current = *head;
  while (current != NULL) {
    next = current->next;
    /*  free(current->file_spec); */
    free(current->info);
    free(current);
    current = next;
  }
  head = NULL;
}

t_list *delete_first_node(t_list *head) {
  t_list *temp = head;
  head = head->next;
  free(temp);
  return head;
}


/* delete any nodes whose filenames start with char value */
t_list *delete_nodes(t_list *file_list, char value) {
  t_list *current = file_list;
  t_list *previous = NULL;
  t_list *next = current->next;
  char *name;

  while (current->next != NULL) {
    name = current->filename;
    if (name[0] == value) {
      if (previous == NULL) {
        // delete current - first node of list.
        file_list = delete_first_node(current);
        current = file_list;
      } else {
        // delete current - a node somewhere in the list.
        previous->next = next;
        t_list *temp = current;
        current = next;
        next = next->next;
        free(temp);
      }
    } else {
      // no match: traverse the list.
      previous = current;
      current = current->next;
      next = next->next;
    }
  }
  return file_list;
}
