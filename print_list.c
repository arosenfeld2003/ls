/*
**
*/
#include <stdlib.h>
#include <stdio.h>

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

// void         print_mod_time(char *filename) {
//   struct stat *info = get_info(filename);
//   time_t mod_time = get_mod_time(info);
//   printf("%llu      ", mod_time);
// }