/*
**
*/
#include "my_list.h"

/*
**
*/


void           my_ls(int argc, char **argv) {
  t_list        *file_list = create_list();
  t_opts        *opts = NULL;
  char          *filename;

  filename = set_filename(argc, argv, filename);
  opts = pass_in_options(argc, argv, opts);
  file_list = make_list(file_list, filename, opts);
  file_list = sort_with_options(file_list, opts);
  print_list(file_list, opts);

  if (opts->list_dirs_recursively) {
    recurse(file_list, opts);
  }

  destroy_list(&file_list);
}