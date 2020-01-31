/*
**
*/
#include "my_list.h"

/*
**
*/

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