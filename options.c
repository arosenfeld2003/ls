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

t_list        *handle_options(t_list *file_list, t_opts *opts) {
  /* handle -a option */
  if (opts->include_hidden_files == 0) {
    /* if no flag, remove all files starting with a dot from list. */
    char dot = '.';
    file_list = delete_nodes(file_list, dot);
  }

  /* handle option t */
  if (opts->sort_by_time_modified == 1) {
    t_list          *head = file_list;
    while (head != NULL) {
      char *filename = ((char *) head->data);
      struct stat *file_info = get_info(filename);
      time_t mod_time = get_mod_time(file_info);
      void *p;
      p = &mod_time;
      head->mod_time = p;
      free(file_info);
      head = head->next;
    }
  }
  return file_list;
}
