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
  new_list->file_size = NULL;
  new_list->mod_time = NULL;
  new_list->is_dir = NULL;
  new_list->next = NULL;
  return new_list;
}

