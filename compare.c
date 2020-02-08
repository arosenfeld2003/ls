/*
**
*/

#include "my_list.h"

/*
**
*/


/* a > b ==> 1; a < b ==> -1; a == b ==> 0; */

int         compare_size(long long a, long long b) {
  return a > b ? 1 : (a < b ? -1 : 0);
}

/* for alphabetical sort */
int         compare_strings(char *a, char*b) {
  int i = 0;
  int comp = compare_size(a[i], b[i]);

  // handle strings that start with the same letter.
  while (comp == 0) {
    i++;
    if (a[i] && b[i]) {
      comp = compare_size(a[i], b[i]);
    } else if (!a[i] && !b[i]) {
      /* strings are exactly equal */
        return 0;
      } else {
        if (a[i]) {
          comp = 1;
        } else {
        comp = 2;
      }
    }
  }
  return comp;
}

int           compare_file_size(struct stat *file1, struct stat *file2) {
  off_t file1_size = file1->st_size;
  off_t file2_size = file2->st_size;
  int comp = compare_size(file1_size, file2_size);
  return comp;
}

int           compare_time(struct stat *file1, struct stat *file2) {
  off_t file1_size = file1->st_mtime;
  off_t file2_size = file2->st_mtime;
  int comp = compare_size(file1_size, file2_size);
  return comp;
}