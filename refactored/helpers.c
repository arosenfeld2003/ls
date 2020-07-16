#include "my_list.h"

/* concatanate dirname and filename with '/' separator */
char *make_path(char *dirname, char *filename) {
  char *path;
  if (strcmp(dirname, ".") == 0 && filename[0] == '.') {
    path = malloc(sizeof(char) * 2);
    strcpy(path, ".");
  } else {
    path = malloc(sizeof(char) * ((strlen(dirname) +
                        strlen(filename)) + 2));
    strcpy(path, dirname);
    strcat(path, "/");
    strcat(path, filename);
  }
  return path;
}

/* copy a node to a new node with next->NULL */
t_list *node_copy(t_list *original) {
  t_list *new = create_node();
  struct stat *buf_info = malloc(sizeof(struct stat));

  new->filename = malloc(sizeof(char) * strlen(original->filename) + 1);
  strcpy(new->filename, original->filename);
  new->path = malloc(sizeof(char) * strlen(original->path) + 1);
  strcpy(new->path, original->path);
  stat(new->filename, buf_info);
  new->info = malloc(sizeof(stat));
  new->info = buf_info;
  new->is_dir = original->is_dir;
  new->is_original = original->is_original;
  new->next = NULL;
  return new;
}