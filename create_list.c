/*
**
*/
#include "my_list.h"

/*
**
*/

t_list          *create_list() {
  t_list        *new_list = malloc(sizeof(t_list));
  new_list->data = NULL;
  new_list->next = NULL;
  return new_list;
}

