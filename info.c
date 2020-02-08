/*
**
*/

#include "my_list.h"

/*
**
*/

int get_info(char *filename, struct stat *buf) {
  if (stat(filename, buf) == 0) {
    /* fill stat buffer with stat of node. */
    return 0;
  }
  return -1;
}

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

int is_directory(char *path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}