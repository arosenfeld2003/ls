
/*
**
*/
#include "./my_list.h"

/*
**
*/

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
