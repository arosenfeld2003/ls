/*
**
*/
#include "my_list.h"

/*
**
*/

void           my_ls(int argc, char **argv) {
  char          **filenames;

  filenames = set_filenames(argc, argv);

  int i = 0;
  while(filenames[i]) {
    t_list        *file_list = create_list();
    t_opts        *opts = NULL;

    /* For the head node, the filename and full pathname are the same */
    file_list->filename = filenames[i];
    file_list->full_pathname = filenames[i];

    file_list->next = NULL;
    opts = pass_in_options(argc, argv, opts);
    file_list = read_directory(file_list, opts);
    file_list = sort_with_options(file_list, opts);

    print_list(file_list, opts);

    /* handle -R option to recursively print directory files */
    if (opts->list_dirs_recursively == 1) {
      while (file_list != NULL) {

        if (can_recurse_dir(file_list->filename)) {
          printf("Recursing into %s\n", file_list->filename);

          recurse(file_list, opts);
          file_list = file_list->next;
        }
      }
    }

    destroy_list(&file_list);
    i++;
  }
}