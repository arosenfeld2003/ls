/*
**
*/

#include "my_list.h"

/*
**
*/

t_list          *append_to_list(t_list *current_list, t_list *new_node) {
  t_list        *head = current_list; // pointer to head of list.

  while (current_list->next != NULL) {
    current_list = current_list->next;
  }
  
  current_list->next = new_node;
  return head;
}