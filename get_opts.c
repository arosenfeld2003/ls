/*
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

/*
**
*/

#include "my_list.h"

/*
**
*/

// initialize without any options.
t_opts *init_opts() {
  t_opts *opts = malloc(sizeof(t_opts));
  opts->include_hidden_files = 0; // -a
  opts->show_size_in_bytes = 0; // -z
  opts->list_dirs_recursively = 0; // -R
  opts->sort_by_time_modified = 0; // -t
  return opts;
}

t_opts get_opts(char *flags, t_opts *opts) {
  int i;
  while (flags[i]) {
    switch(flags[i]) {
      case 'a':
        opts->include_hidden_files = 1;
        break;
      case 'z':
        opts->show_size_in_bytes = 1;
        break;
      case 'R':
        opts->list_dirs_recursively = 1;
        break;
      case 't':
        opts->sort_by_time_modified = 1;
        break;
    }
    i++;
  }
  return *opts;
}


// testing.
int main(int argc, char**argv) {
  char *flags = argv[1];
  t_opts *opts = init_opts();

  if (flags[0] == '-') {
    flags++;
    *opts = get_opts(flags, opts);
  }

  printf("%d\n", opts->include_hidden_files); // -a
  printf("%d\n", opts->show_size_in_bytes); // -z
  printf("%d\n", opts->list_dirs_recursively); // -R
  printf("%d\n", opts->sort_by_time_modified); // -t

  free(opts);
}
