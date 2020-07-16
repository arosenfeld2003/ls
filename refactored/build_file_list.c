#include "my_list.h"

/* create an empty node */
t_list *create_node() {
  t_list *new_node = malloc(sizeof(t_list));
  new_node->filename = NULL;
  new_node->path = NULL;
  new_node->info = malloc(sizeof(struct stat));
  new_node->is_dir = 0;
  new_node->is_original = 0;
  new_node->next = NULL;
  return new_node;
}

/* files from user input */
t_list *make_user_filelist(int argc, char **argv) {
  t_list *file_list = create_node();
  t_list *current_node = file_list;

  char **filenames = set_filenames(argc, argv);
  int file_count = count_files(argc, argv);

  int i = 0;
  while (i < file_count) {
    if (current_node->filename != NULL) {
      current_node->next = create_node();
      current_node = current_node->next;
    }
    current_node->filename = filenames[i];
    current_node->path = filenames[i];
    current_node->path = make_path(".", filenames[i]);
    if (stat(current_node->filename, current_node->info) == -1) {
      current_node = current_node->next;
      i++;
    } else {
      if (S_ISDIR(current_node->info->st_mode)) {
        current_node->is_dir = 1;
      }
      i++;
    }
    current_node->is_original = 1;
  }
  free(filenames);
  return file_list;
}

t_list *make_dir_list(t_list *dir_node) {
  // recreate the node, with next->null
  t_list *head_dir = node_copy(dir_node);
  t_list *current = head_dir;
  struct dirent *buf_dir;
  // open dir!
  DIR *directory = opendir(head_dir->path);
  if (directory != NULL) { /* be sure we have a valid directory */
    /* continually open next directory stream until reaching the end of file list. */
    while ((buf_dir = readdir(directory)) != NULL) {
      // create next node
      current->next = create_node();
      current = current->next;
      current->filename = malloc(sizeof(char) * strlen(buf_dir->d_name) + 1);
      strcpy(current->filename, buf_dir->d_name);
      char *path = make_path(head_dir->path, current->filename);
      current->path = malloc(sizeof(char) * strlen(path) + 1);
      strcpy(current->path, path);
      struct stat *buf_stat = malloc(sizeof(struct stat));
      stat(current->path,  buf_stat);
      current->info = buf_stat;
      if (S_ISDIR(current->info->st_mode)) {
        current->is_dir = 1;
      }
    }
  }
  closedir(directory);
  // skip the head_dir node and return root of the directory.
  return head_dir->next;
}