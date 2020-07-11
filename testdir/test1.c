#include <stdio.h>
#include <stdlib.h>

int count_filenames(int argc, char **argv) {
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

int main(int argc, char **argv) {
  int filecount = count_filenames(argc, argv);
  printf("Filecount is: %d\n", filecount);
  return 0;
}
