#include "my_list.h"

void free_node(t_list *node) {
  /* in the original nodes, *path == *filename */
  if (node->filename != node->path) {
    free(node->filename);
  }
  free(node->path);
  free(node->info);
  free(node);
  node = NULL;
}

void destroy_list(t_list **head) {
  t_list *current = *head;
  while (current != NULL) {
    t_list *temp = current;
    current = current->next;
    free_node(temp);
  }
  head = NULL;
}