/*
**
*/

#include "my_list.h"

/*
**
*/

void          print_list(t_list *head) {
  t_list *current_node = head;
  while (current_node != NULL) {
    char *filename = ((char *) current_node->data); // cast the data to a string.
    printf("%s", filename);
    printf("\n");
    current_node = current_node->next;
  }
}

