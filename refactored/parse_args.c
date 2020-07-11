#include "my_list.h"

int count_files(int argc, char **argv) {
  if (argc == 1) {
    return 1;
  }
  int filecount = 0;
  int i = 1;
  while (i < argc) {
    if (argv[i][0] != '-') {
      filecount++;
    }
    i++;
  }
  return filecount;
}

/* user must free filenames array */
char **set_filenames(int argc, char **argv) {
  char **filenames = NULL;
  char *file = NULL;
  int file_count = count_files(argc, argv);

  if ((argc == 1) || (argc == 2 && argv[1][0] == '-')) {
    file_count = 1;
    file = ".";
  } else {
    int i = 1;
    // skip the flags.
    while (argv[i][0] == '-') {
      i++;
    }
  }
  filenames = malloc(sizeof(char *) * (file_count));
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

char *create_flags(int argc, char **argv) {
  char *flags = NULL;
  int flag_count = 0;

  // count number of flags and allocate space.
  int i = 1;
  while (i < argc) {
    if (argv[i][0] ==  '-') {
      flag_count += (strlen(argv[i]) - 1);
    }
    i++;
  }
  flags = malloc(sizeof(char) * flag_count + 1);
  char temp_head[flag_count + 1];
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
