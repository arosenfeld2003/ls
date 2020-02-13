/*
**
*/
#include "my_list.h"

/*
**
*/

bool            can_recurse_dir(char *curr) {
  if (!strcmp(".", curr) || !strcmp("..", curr)) {
    return false;
  }
  return true;
}

void            recurse(t_list *recursive_list, t_opts *opts) {
  /* TODO: ignore hidden directories unless option -a */

  char           *path = recursive_list->full_pathname;
  DIR            *directory = opendir(path);
  struct dirent  *buf_spec;

  if (directory == NULL) {
    printf("%s is not a directory.\n", path);
  } else {
    /* continually open next directory stream until reaching the end of file list. */
    while ((buf_spec = readdir(directory)) != NULL) {
      char *filename = buf_spec->d_name;
      char *full_path = get_full_pathname(path, filename);
      recursive_list = create_new_node(recursive_list, filename, full_path);
    }
    recursive_list = sort_with_options(recursive_list, opts);
    print_list(recursive_list, opts);
    closedir(directory);
  }
  printf("Recursing into %s.\n", recursive_list->full_pathname);
}