/*
**
*/

#include "my_list.h"

/*
**
*/

/* total size, in bytes */
off_t get_size(struct stat *file_info) {
  return file_info->st_size;
}

/* convert struct time_t to struct tm ==> gives access to local time */
struct tm *format_time(time_t time) {
  struct tm *formatted = malloc(sizeof(struct tm));
  localtime_r(&time, formatted); /* method from lib time.h */
  return formatted;
}

/* malloc - free'd in destroy-node method */
char *get_full_pathname(char *path, char *filename) {

  /* skip pathnames of the current and parent directories */
  if (strcmp(path, filename) == 0) {
    return filename;
  }

  size_t        path_length = strlen(filename);
  char *fullpath = malloc(path_length + strlen(path) + 2);
  if (fullpath == NULL) { /* deal with error and exit */ }

  /* handle case of parent directory ==> " ../ " */
  if (strcmp(path, "../") == 0) {
    sprintf(fullpath, "%s%s", path, filename);
  } else {
    sprintf(fullpath, "%s/%s", path, filename);
  }
  return fullpath;
}