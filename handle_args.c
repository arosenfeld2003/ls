/*
**
*/

#include "my_list.h"

/*
**
*/

char            *set_filename(int argc, char **args, char *filename) {
  if (argc == 1 || args[1][0] == '-') {
    filename = ".";
  } else {
    filename = args[1];
  }
  return filename;
}

t_opts          *pass_in_options(int argc, char **args, t_opts *opts) {
  if (argc > 1) {
    opts = create_opts(args, 1);
  } else {
    opts = create_opts(args, 0);
  }
  return opts;
}