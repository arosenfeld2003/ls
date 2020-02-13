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
  new_list->filename = NULL;
  new_list->info = malloc(sizeof(struct stat));
  new_list->is_dir = 0;
  new_list->next = NULL;
  return new_list;
}


t_list          *create_new_node(t_list *head, char *filename, char *full_pathname) {
  /* struct stat *buf is freed in delete_nodes method */
  if (strcmp(head->filename, filename) == 0) {
    return head;
  } else {
    /* Create a new node and append it to the list */
    t_list        *new_node = malloc(sizeof(t_list));
    struct stat   *buf_info = malloc(sizeof(struct stat));
    new_node->filename = filename;
    new_node->full_pathname = full_pathname;
    /* use full_pathname for stat? */
    stat(new_node->full_pathname, buf_info);
    new_node->info = buf_info;
    new_node->next = NULL;
    head = append_to_list(head, new_node);
  }

  return head;
}

t_list           *read_directory(t_list *head, t_opts *opts) {
  char           *pathname = head->full_pathname;
  DIR            *directory = opendir(pathname);


  if (directory == NULL) {
    return head;
  } else {
    struct dirent  *buf_spec = malloc(sizeof(struct dirent));

    /* continually open next directory stream until reaching the end of file list. */
    while ((buf_spec = readdir(directory)) != NULL) {

      char *filename = buf_spec->d_name;

      /* use fullpath - make sure to free it */
      char *full_path = get_full_pathname(pathname, filename);

      // struct dirent *file_spec = malloc(sizeof(struct dirent));
      // file_spec = buf_spec;

      head = create_new_node(head, filename, full_path);
    }
    closedir(directory);
  }
  return head;
}