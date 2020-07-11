#include "my_list.h"

/* create an empty node */
t_list *create_node() {
  t_list *new_node = malloc(sizeof(t_list));
  new_node->filename = NULL;
  new_node->path = NULL;
  new_node->info = NULL;
  new_node->is_dir = 0;
  new_node->next = NULL;
  return new_node;
}

/* files from user input */
t_list *make_user_filelist(int argc, char **argv) {
  t_list *file_list = create_node();
  t_list *current_node = file_list;

  struct stat *buf_info = malloc(sizeof(struct stat));
  char **filenames = set_filenames(argc, argv);
  int file_count = count_files(argc, argv);

  int i = 0;
  while (i < file_count) {
    if (current_node->filename != NULL) {
      current_node->next = create_node();
      current_node = current_node->next;
    }
    current_node->filename = filenames[i];
    if (strcmp(current_node->filename, ".") == 0) {
      current_node->path = filenames[i];
    }
    if (stat(current_node->filename, buf_info) == -1) {
      i++;
    } else {
      if (S_ISDIR(buf_info->st_mode)) {
        current_node->is_dir = 1;
      }
      current_node->info = malloc(sizeof(struct stat));
      current_node->info = buf_info;
      i++;
    }
  }
  free(filenames);
  free(buf_info);
  return file_list;
}
