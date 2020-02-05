/*
**
*/

#include "./my_list.h"

/*
**
*/


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

/* Merge two sorted lists into one list, in alphabetical order based on filename. */
t_list        *alpha_sorted_merge(t_list *a, t_list *b) {
  t_list      *result = NULL;

  /* Base cases */
  if (a == NULL)
    return (b);
  else if (b == NULL)
    return (a);

  /* cast data from void pointer to a char */
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

/* merge lists in sorted order based on time modified */
t_list        *time_mod_sorted_merge(t_list *a, t_list *b) {
  t_list      *result = NULL;

  /* Base cases */
  if (a == NULL) {
    return (b);
  }

  else if (b == NULL) {
    return (a);
  }

  /* get time mod */
  long mod_time_a = a->file_info->st_mtime;
  long mod_time_b = b->file_info->st_mtime;

  /* Pick either a or b, and recur */
  if (compare_size(mod_time_a, mod_time_b) == 1 ||
    compare_size(mod_time_a, mod_time_b) == 0) {
      result = a;
      result->next = time_mod_sorted_merge(b, a->next);
    } else {
    result = b;
    result->next = time_mod_sorted_merge(b->next, a);
  }
  return (result);
}

void          merge_sort(t_list **headRef, t_opts *opts) {
  t_list      *head = *headRef;
  t_list      *a;
  t_list      *b;

  /* Base case -- length 0 or 1 */
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
