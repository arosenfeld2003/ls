#include "./my_list.h"

/* a > b ==> 1; a < b ==> -1; a == b ==> 0; */
int compare_size(int a, int b) {
  return a > b ? 1 : (a < b ? -1 : 0);
}

/* for alphabetical sort */
int compare_strings(char *a, char *b) {
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

int compare_file_size(struct stat *file1, struct stat *file2) {
  off_t file1_size = file1->st_size;
  off_t file2_size = file2->st_size;
  int comp = compare_size(file1_size, file2_size);
  return comp;
}

int compare_time(struct stat *file1, struct stat *file2) {
  off_t file1_size = file1->st_mtime;
  off_t file2_size = file2->st_mtime;
  int comp = compare_size(file1_size, file2_size);
  return comp;
}

/*
  Split nodes of given list into front and back halves, and return the two lists.
  If source length is odd, the extra node goes in the front list.
  Use "fast/slow" pointer strategy.
*/
void split_node(t_list *source, t_list **front, t_list** back) {
  t_list *slow = source;
  t_list *fast = source->next;
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
  *back = slow->next;
  slow->next = NULL;
}


/* Merge two sorted lists into one list, in alphabetical order based on filename. */
t_list *alpha_sorted_merge(t_list *a, t_list *b) {
  t_list *result = NULL;
  /* Base cases */
  if (a == NULL)
    return (b);
  else if (b == NULL)
    return (a);
  /* Pick either a or b, and recur */
  if (compare_strings(a->filename, b->filename) == 1 ||
    compare_strings(a->filename, b->filename) == 0) {
      result = b;
      result->next = alpha_sorted_merge(a, b->next);
    } else {
    result = a;
    result->next = alpha_sorted_merge(a->next, b);
  }
  return (result);
}

/* merge lists in sorted order based on time modified */
t_list *time_mod_sorted_merge(t_list *a, t_list *b) {
  t_list *result = NULL;
  /* Base cases */
  if (a == NULL) {
    return (b);
  } else if (b == NULL) {
    return (a);
  }
  if (compare_size((long long)a->info->st_mtime, (long long)b->info->st_mtime) == 1) {
    result = a;
    result->next = time_mod_sorted_merge(b, a->next);
  } else {
    result = b;
    result->next = time_mod_sorted_merge(b->next, a);
  }
  return (result);
}


/* merge lists in sorted order based on time modified */
t_list *file_size_sorted_merge(t_list *a, t_list *b) {
  t_list *result = NULL;
  /* Base cases */
  if (a == NULL) {
    return (b);
  } else if (b == NULL) {
    return (a);
  }
  if (compare_size((long long)a->info->st_size, (long long)b->info->st_size) == 1 ||
        compare_size((long long)a->info->st_size, (long long)b->info->st_size) == 0) {
    result = a;
    result->next = file_size_sorted_merge(b, a->next);
  } else {
    result = b;
    result->next = file_size_sorted_merge(b->next, a);
  }
  return (result);
}

void merge_sort(t_list **headRef, t_opts *opts) {
  t_list *head = *headRef;
  t_list *a;
  t_list *b;
  /* Base case -- length 0 */
  if ((head == NULL) || (head->next == NULL)) {
    return;
  }
  /* Split head into 'a' and 'b' sublists */
  split_node(head, &a, &b);
  /* Recursively break down the sublists */
  merge_sort(&a, opts);
  merge_sort(&b, opts);
  if (opts && opts->sort_by_time_modified == 1) {
    *headRef = time_mod_sorted_merge(a, b);
  } else {
    /* merge lists sorted alphabetically */
    *headRef = alpha_sorted_merge(a, b);
  }
}

t_list *sort_with_options(t_list *file_list, t_opts *opts) {
  /* handle options */
  merge_sort(&file_list, opts);
  return file_list;
}