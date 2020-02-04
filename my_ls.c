/*
**
*/

#include "./my_list.h"

/*
**
*/


// testing main
int main(int argc, char **argv) {
  // For now, only accepts flags and/or one file arg

  // check for flags and options
  char              *flags = argv[1];
  char              *filename;
  t_opts            *opts = init_opts();

  if (flags[0] == '-') {
    flags++;
    opts = get_opts(flags, opts);
    filename = argv[2];
  } else {
    filename = argv[1];
  }

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

  return 0;
}


