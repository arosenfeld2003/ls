/*
**
*/
#include "my_list.h"

/*
**
*/

/* create an empty list */
t_list          *create_list() {
  t_list        *new_list = malloc(sizeof(t_list));
  new_list->data = NULL;
  new_list->info = malloc(sizeof(struct stat));
  new_list->is_dir = 0;
  new_list->next = NULL;
  return new_list;
}

void           add_info(t_list *node, struct stat *buf) {
  char *filename = ((char *)node->data);
  get_info(filename, buf);
  node->info = buf;
}

t_list          *create_new_node(t_list *head, int is_dir, void *data, size_t data_size) {
  struct stat *buf = malloc(sizeof(struct stat));
  /* Empty list ==> add data to the only existing node */
  if (head->data == NULL) {
    head->data = malloc(sizeof(data_size));
    head->data = data;
    add_info(head, buf);
  } else {
    /* Create a new node and append it to the list */
    t_list        *new_node = malloc(sizeof(t_list));
    new_node->data = malloc(sizeof(data_size));
    new_node->data = data;
    new_node->is_dir = is_dir;
    add_info(new_node, buf);
    new_node->next = NULL;

    t_list *current = head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_node;
  }
  return head;
}

t_list          *add_node(t_list *head, char *filename) {
  void *pathname;
  int is_dir = is_directory(filename) ? 0 : 1;
  pathname = filename;
  head = create_new_node(head, is_dir, filename, sizeof(char *));
  return head;
}

t_list           *read_directory(t_list *head, t_opts *opts) {
  char           *path = ((char *) head->data);
  DIR            *directory = opendir(path);
  struct dirent  *file = NULL;
  t_list         *file_list = create_list();

  /* continually open next directory stream until reaching the end of file list. */
  while ((file = readdir(directory)) != NULL) {
    char *filename = file->d_name;
    file_list = add_node(file_list, filename);
  }
  closedir(directory);
  return file_list;
}

t_list            *make_list(t_list *file_list, char *filename, t_opts *opts) {
  file_list = add_node(file_list, filename);
  file_list = read_directory(file_list, opts);
  return file_list;
}