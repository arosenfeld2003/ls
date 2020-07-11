#include "my_list.h"

void free_node(t_list *node) {
  free(node->filename);
  free(node->path);
  free(node->info);
  free(node);
  node = NULL;
}

void destroy_list(t_list **head) {
  t_list        *current = *head;
  while (current != NULL) {
    t_list *temp = current;
    current = current->next;
    free_node(temp);
  }
  head = NULL;
}

t_list *delete_first_node(t_list *head) {
  t_list *temp = head;
  head = head->next;
  free_node(temp);
  return head;
}

/* delete any nodes whose filenames start with char value */
t_list *delete_node(t_list *file_list, char *filename) {
  t_list *current = file_list;
  t_list *previous = NULL;

  while (current != NULL) {
    if (strcmp(current->filename, filename) == 0) {
      if (previous == NULL) {
        // delete current - first node of list.
        file_list = delete_first_node(current);
        current = file_list;
      } else {
        // delete current - a node somewhere in the list.
        previous->next = current->next;
        t_list *temp = current;
        current = current->next;
        free_node(temp);
      }
    } else {
      // no match: traverse the list.
      previous = current;
      current = current->next;
    }
  }
  return file_list;
}
