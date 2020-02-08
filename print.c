/*
**
*/

#include "my_list.h"

/*
**
*/

char* formatdate(char* str, time_t val) {
  strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
  return str;
}

void          print_list(t_list *head, t_opts *opts) {
  t_list *current_node = head;
  while (current_node != NULL) {
    char *filename = ((char *) current_node->data); // cast the data to a string.
    printf("%s", filename);

    if (opts->sort_by_time_modified) {
      char *buf;
      buf = ctime(&current_node->info->st_mtime);
      printf(", time modified: %s", buf);
    }

    if (opts->show_size_in_bytes) {
      printf(", size: %lld", ((long long)current_node->info->st_size));
    }

    printf("\n");
    current_node = current_node->next;
  }
}
