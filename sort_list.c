#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

/*
**
*/
#include "my_list.h"

/*
**
*/

t_list          *create_list() {
  t_list        *new_list = malloc(sizeof(t_list));
  new_list->data = NULL;
  new_list->next = NULL;
  return new_list;
}

void            print_list(t_list *head) {
  t_list        *current_node = head;

  while (current_node != NULL) {
    char *filename = ((char *) current_node->data); // cast the data to a string.
    printf("%s\n", filename);
    current_node = current_node->next;
  }
}

t_list          *append_to_list(t_list *current_list, t_list *new_node) {
  t_list        *head = current_list; // pointer to head of list.

  if (current_list->data == NULL) {
    errno = -1;
  } else {
    while (current_list->next != NULL) {
      current_list = current_list->next;
    }
    current_list->next = new_node;
  }

  current_list = head; // reset pointer to start of list.
  return head;
}

void            destroy_list(t_list **head) {
  t_list        *current;
  t_list        *next;

  current = *head;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  head = NULL;
}

t_list          *create_file_list(DIR *dirp, char *filename) {
  t_list *head = create_list();
  struct dirent *file;

  // continually open next directory stream until reaching the end of file list.
  while ((file = readdir(dirp)) != NULL) {
    char *name = file->d_name;
    void *p;
    p = name;

    if (head->data == NULL) {
      // empty list.
      head->data = p;
    } else {
      t_list *new_node = create_list();
      new_node->data = p;
      head = append_to_list(head, new_node);
    }
  }
  closedir(dirp);
  return head;
}

/* Helper Functions for Merge Sort */

/*
  a > b ==> 1;
  a < b ==> -1;
  a == b ==> 0;
*/

int         compare_size(long long a, long long b) {
  return a > b ? 1 : (a < b ? -1 : 0);
}

int         compare_strings(char *a, char*b) {
  int i = 0;
  int comp = compare_size(a[i], b[i]);

  // handle strings that start with the same letter.
  while (comp == 0) {
    i++;
    if (a[i] && b[i]) {
      /* keep comparing size of next letter */
      comp = compare_size(a[i], b[i]);
    } else if (!a[i] && !b[i]) {
      /* strings are exactly equal */
      return 0;
    } else {
      if (a[i]) {
        /* string a is longer */
        comp = 1;
      } else {
        /* string b is longer */
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

/*
  Split nodes of given list into front and back halves, and return the two lists.
  If source length is odd, the extra node goes in the front list.
  Use "fast/slow" pointer strategy.
*/
void          split_node(t_list *source, t_list **front, t_list** back) {
  t_list      *fast;
  t_list      *slow;

  slow = source; 
  fast = source->next;

  /* Advance 'fast' two nodes, and advance 'slow' one node */
  while (fast != NULL) {
    fast = fast->next;
    if (fast != NULL) {
      slow = slow->next;
      fast = fast->next;
    }
  }

  /* 'slow' is before the midpoint in the list - split at that point. */
  *front = source;
  *back= slow->next;
  slow->next = NULL;
}

/*
  Merge two sorted lists into one list, in alphabetical order based on filename.
*/
t_list        *alpha_sorted_merge(t_list *a, t_list *b) {
  t_list      *result = NULL;

  /* Base cases */
  if (a == NULL)
    return (b);
  else if (b == NULL)
    return (a);

  /* cast data from void pointer to an int */
  char *filename_a = ((char *) a->data);
  char *filename_b = ((char *) b->data);

  /* Pick either a or b, and recur */
  if (compare_strings(filename_a, filename_b) == 1 ||
    compare_strings(filename_a, filename_b) == 0) {
      result = b;
      result->next = alpha_sorted_merge(a, b->next);
    } else {
    result = a;
    result->next = alpha_sorted_merge(a->next, b);
  }
  return (result);
}


void        merge_sort(t_list **headRef) {
  t_list    *head = *headRef;
  t_list    *a;
  t_list    *b;

  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL)) {
    return;
  }

  /* Split head into 'a' and 'b' sublists */
  split_node(head, &a, &b);

  /* Recursively break down the sublists */
  merge_sort(&a);
  merge_sort(&b);

  /* merge the two sorted lists together */
  *headRef = alpha_sorted_merge(a, b);
}

// testing.
int main(int argc, char **argv) {
  char          *filename = argv[1];
  t_list        *file_list;
  DIR           *dirp = opendir(filename);

  file_list = create_file_list(dirp, filename);

  // print_list(file_list);

  merge_sort(&file_list);

  print_list(file_list);

  destroy_list(&file_list);

  return 0;
}
