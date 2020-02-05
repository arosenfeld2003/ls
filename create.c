/*
**
*/
#include "my_list.h"

/*
**
*/

/* create an empty list */
t_list          *create_list() {
  t_list        *new_list = malloc(sizeof(t_list));
  new_list->data = NULL;
  new_list->file_info = NULL;
  new_list->next = NULL;
  return new_list;
}


/* create list when passed a filename that isn't a directory */
t_list          *create_name_only_list(char *filename) {
  t_list *head = create_list();
  // head = set_is_hidden(head, filename);
  void *p;
  p = filename;
  head->data = p;
  return head;
}

/* create list for a directory */
t_list          *create_file_list(DIR *dirp, char *filename) {
  t_list *head = create_list();
  // head = set_is_hidden(head, filename);
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

