/*
**
*/

#include "my_list.h"

/*
**
*/


// testing main
int main(int argc, char **argv) {
  char *filename;
  t_opts *opts = NULL;
  int i = 1;

  filename = set_filename(argc, argv, filename);
  opts = pass_in_options(argc, argv, opts);

  while (filename) {
    struct stat       *file_info = get_info(filename);
    t_list            *file_list;
    DIR               *dirp = opendir(filename);

    if (S_ISREG(file_info->st_mode)) {
      file_list = create_name_only_list(filename);
    } else if (S_ISDIR(file_info->st_mode)) {
      file_list = create_file_list(dirp, filename);
    }

    file_list = handle_options(file_list, opts);

    merge_sort(&file_list, opts);
    print_list(file_list);
    free(file_info);
    destroy_list(&file_list);

    i++;
    filename = argv[i];
  }
  return 0;
}