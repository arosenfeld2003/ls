/*
**
*/
#include <stdlib.h>
#include <stdio.h>
#include<errno.h>

/*
**
*/
#include "my_list.h"

/*
**
*/

t_list          *prepend_to_list(t_list *current_list, t_list *new_node) {
  t_list        *head = current_list; // pointer to head of list.

  if (new_node->data == NULL) {
    errno = -1;
  } else {
    t_list *head = new_node;
    head->next = current_list;
  }
  return head;
}