#include "my_list.h"

/* concatanate dirname and filename with '/' separator */
char *make_path(char *dirname, char *filename) {
  char *path;
  if (dirname[0] == '.') {
    path = malloc(sizeof(char) * strlen(dirname) + 1);
    strcpy(path, dirname);
  } else {
    path = malloc(sizeof(char) * ((strlen(dirname) +
                        strlen(filename)) + 2));
    strcpy(path, dirname);
    strcat(path, "/");
    strcat(path, filename);
  }
  return path;
}

/* copy a node to a new node with next->NULL */
t_list *node_copy(t_list *original) {
  t_list *new = create_node();
  struct stat *buf_info = malloc(sizeof(struct stat));

  new->filename = malloc(sizeof(char) * strlen(original->filename) + 1);
  strcpy(new->filename, original->filename);
  new->path = malloc(sizeof(char) * strlen(original->path) + 1);
  strcpy(new->path, original->path);
  stat(new->filename, buf_info);
  new->info = malloc(sizeof(stat));
  new->info = buf_info;
  new->is_dir = original->is_dir;
  new->is_original = original->is_original;
  new->next = NULL;
  return new;
}

/* convert struct time_t to struct tm ==> gives access to local time */
struct tm *format_time(time_t time) {
  struct tm *formatted = malloc(sizeof(struct tm));
  localtime_r(&time, formatted); /* method from lib time.h */
  return formatted;
}