/*
**
*/
#include <stdlib.h>
#include <stdio.h>

/*
  a > b ==> 1;
  a < b ==> -1;
  a == b ==> 0;
*/

int         compare_size(int a, int b) {
  return a > b ? 1 : (a < b ? -1 : 0);
}

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

int main(int argc, char**argv) {
  char *a = argv[1];
  char *b = argv[2];
  int comp = compare_strings(a, b);

  printf("%d\n", comp);

  return comp;
}