/*
**
*/

#include "my_list.h"

/*
**
*/

char            **set_filenames(int argc, char **args) {
  char **filenames = malloc(sizeof(char *) * argc);

  if ((argc == 1) || (argc == 2 && args[1][0] == '-')) {
    filenames[0] = ".";
  } else if (argc > 1) {
    int i = 0;
    int j;
    switch(args[1][0]) {
      case '-':
        j = 2;
        break;
      default:
        j = 1;
    }
    while (j <= argc) {
      filenames[i] = args[j];
      i++;
      j++;
    }
  }
  return filenames;
}

t_opts          *pass_in_options(int argc, char **args, t_opts *opts) {
  if (argc > 1) {
    opts = create_opts(args, 1);
  } else {
    opts = create_opts(args, 0);
  }
  return opts;
}