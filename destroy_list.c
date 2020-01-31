/*
**
*/
#include "my_list.h"

/*
**
*/
void          destroy_list(t_list **head) {
  t_list      *current;
  t_list      *next;

  current = *head;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  head = NULL;
}

