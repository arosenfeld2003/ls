#include "my_list.h"

t_list *create_new_node(t_list *head, char *filename, char *full_pathname) {
  /* Do not create a duplicate of head node */
  if (strcmp(head->filename, filename) == 0) {
    return head;
  } else {
    /* Create a new node and append it to the list */
    t_list *new_node = malloc(sizeof(t_list));
    struct stat *buf_info = malloc(sizeof(struct stat));
    new_node->filename = malloc(sizeof(char) * strlen(filename) + 1);
    strcpy(new_node->filename, filename);
    new_node->full_pathname = malloc(sizeof(char) * strlen(full_pathname) + 1);
    strcpy(new_node->full_pathname, full_pathname);

    /* use full_pathname for stat? */
    stat(new_node->full_pathname, buf_info);
    new_node->info = buf_info;
    new_node->next = NULL;
    head = append_to_list(head, new_node);
  }

  return head;
}

t_list *read_directory(t_list *head) {
  char *pathname = head->full_pathname;
  DIR *directory = opendir(pathname);

  if (directory == NULL) {
    return head;
  } else {
    struct dirent *buf_spec = NULL;

    /* continually open next directory stream until reaching the end of file list. */
    while ((buf_spec = readdir(directory)) != NULL) {

      char *filename = buf_spec->d_name;

      /* use fullpath - make sure to free it */
      char *full_path = get_full_pathname(pathname, filename);

      head = create_new_node(head, filename, full_path);
    }
    closedir(directory);
  }
  return head;
}