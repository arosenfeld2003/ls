#include "my_list.h"
void print_directories(t_list *file_list, t_opts *opts) {
  struct stat     *buf_stat = malloc(sizeof(struct stat));
  t_list          *head = file_list;
  DIR             *directory = opendir(file_list->path);
  struct dirent   *buf_dir;

  if (directory != NULL) {
    /* continually open next directory stream until reaching the end of file list. */
    while ((buf_dir = readdir(directory)) != NULL) {
      /* build list of files in the directory */
      file_list->next = create_node();
      file_list = file_list->next;
      file_list->filename = buf_dir->d_name;
      realpath(buf_dir->d_name, file_list->path);
      stat(file_list->path, buf_stat);
      if (S_ISDIR(buf_stat->st_mode)) {
        file_list->is_dir = 1;
      }
      file_list->info = buf_stat;
    }
    closedir(directory);
    printf("%s: \n", head->path);
    t_list *files = head->next;
    files = sort_with_options(files, opts);
    print_list(files, opts);
    // recurse to print all directories
    while (files != NULL) {
      if (files->is_dir == 1 && opts->include_hidden_files == 1) {
        print_directories(files, opts);
      }
    }
  }
  file_list = file_list->next;
  if (file_list != NULL) {
    print_directories(file_list, opts);
  }
}

char *formatdate(char *str, time_t val) {
  strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
  return str;
}

void print_list(t_list *head, t_opts *opts) {
  t_list *current_node = head;
  while (current_node != NULL) {
    if (current_node->info == NULL) {
      printf("my_ls: %s: No such file or directory\n", current_node->filename);
    } else {
      /* handle -a option */
      current_node = skip_hidden_files(current_node, opts);
      if (current_node != NULL) {
        printf("%s", current_node->filename);

        if (opts->sort_by_time_modified) {
          char *buf;
          buf = ctime(&current_node->info->st_mtime);
          printf(", time modified: %s", buf);
        }
        if (opts->show_size_in_bytes) {
          printf(", size: %lld", ((long long)current_node->info->st_size));
        }
        printf("\n");
      }
    }
    if (current_node != NULL) {
      current_node = current_node->next;
    }
  }
}