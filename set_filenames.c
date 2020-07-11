#include "my_list.h"

/* user must free filenames array */
char **set_filenames(int argc, char **argv) {
  char **filenames = NULL;
  char *file = NULL;
  int file_count = 0;

  if ((argc == 1) || (argc == 2 && argv[1][0] == '-')) {
    file_count = 1;
    file = ".";
  } else {
    int i = 1;
    // skip the flags.
    while (argv[i][0] == '-') {
      i++;
    }
    // count files.
    while (i < argc) {
      file_count++;
      i++;
    }
  }

  filenames = malloc(sizeof(char *) * file_count);

  if (file != NULL) {
    filenames[0] = malloc(sizeof(char) * strlen(file) + 1);
    strcpy(filenames[0], file);
  } else {
    int i = 0;
    int j;
    switch(argv[1][0]) {
      case '-':
        j = 2;
        break;
      default:
        j = 1;
    }
    while (j < argc) {
      filenames[i] = malloc(sizeof(char) * strlen(argv[j]) + 1);
      strcpy(filenames[i], argv[j]);
      i++;
      j++;
    }
  }
  return filenames;
}