#include "my_list.h"

char *create_flags(int argc, char **argv) {
  char *flags = NULL;
  int flags_len = 0;
  int flag_count = 0;

  // count number of flags and allocate space.
  int i = 1;
  while (i < argc) {
    if (argv[i][0] ==  '-') {
      flag_count += (strlen(argv[i]) - 1);
    }
    i++;
  }
  flags_len = flag_count + 1;
  flags = malloc(sizeof(char) * flags_len);

  char temp_head[flags_len];
  char *temp_ptr = temp_head;
  int len, j;
  i = 1;

  while (i < argc) {
    j = 0;
    if (argv[i][j] == '-') {
      j++;
      len = strlen(argv[i]);
      while (j < len) {
        *temp_ptr = argv[i][j];
        temp_ptr++;
        j++;
      }
    }
    i++;
  }
  *temp_ptr = '\0';
  strcpy(flags, temp_head);

  return flags;
}

t_opts *get_opts(char *flags, t_opts *opts) {
  int i = 0;
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
      default:
        printf("option %c invalid\n", flags[i]);
    }
    i++;
  }
  return opts;
}

// initialize without any options.
t_opts *init_opts() {
  t_opts *opts = malloc(sizeof(t_opts));

  opts->include_hidden_files = 0; // -a
  opts->show_size_in_bytes = 0; // -z
  opts->list_dirs_recursively = 0; // -R
  opts->sort_by_time_modified = 0; // -t

  return opts;
}

t_opts *create_opts(int argc, char **argv) {
  t_opts *opts = init_opts();
  char *flags = create_flags(argc, argv);
  return get_opts(flags, opts);
}

int main (int argc, char **argv) {
  t_opts *opts = create_opts(argc, argv);
  printf("%i\n", opts->include_hidden_files); // -a
  printf("%i\n", opts->show_size_in_bytes); // -z
  printf("%i\n", opts->list_dirs_recursively); // -R
  printf("%i\n", opts->sort_by_time_modified); // -t
  return 0;
}