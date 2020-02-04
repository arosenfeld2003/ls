/*
**
*/
#include "./my_list.h"

/*
**
*/


/* create list when passed a filename that isn't a directory */
t_list          *create_name_only_list(char *filename) {
  t_list *head = create_list();
  head = set_is_hidden(head, filename);
  void *p;
  p = filename;
  head->data = p;
  return head;
}

/* create list for a directory */
t_list          *create_file_list(DIR *dirp, char *filename) {
  t_list *head = create_list();
  head = set_is_hidden(head, filename);
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


// testing.
// int main(int argc, char **argv) {
//   char              *filename = argv[1];
//   struct stat       *file_info = get_stat(filename);
//   t_list            *file_list;
//   DIR               *dirp = opendir(filename);

//   if (S_ISREG(file_info->st_mode)) {
//     file_list = create_name_only_list(filename);
//   } else if (S_ISDIR(file_info->st_mode)) {
//     file_list = create_file_list(dirp, filename);
//   }
//   print_list(file_list);
//   free(file_info);
//   destroy_list(&file_list);

//   return 0;
// }

// works for when passed a directory!!
// TODO:
// 1. if not a directory, print filename ==> done, 1/31 (NOT A LIST).
// 2. add necessary stat info to the void pointer data for each list node.
// 3. SORT LIST FUNCTIONS - OPTIONS ==>
  // sort alphabetically ==> done, 1/30.
  // create a way to accept options.
  // -a : include files starting with a "."
  // -t : sort/display time modified.
  // -R : recursively list sub-directories.
  // -z : display number of bytes in each file.


  // struct stat   *file_stats = get_stat(filename);
  // if ((S_ISDIR(file_stats->st_mode))) {
  // } else {
  //   void *p;
  //   p = filename;
  //   file_list->data = p;
  // }
  // free(file_stats);
