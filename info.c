/*
**
*/

#include "my_list.h"

/*
**
*/

struct stat *get_info(char *filename) {
  struct stat *buf = malloc(sizeof(stat)); /* malloc a buffer for the stat. */
  stat(filename, buf); /* fill stat buffer with stat of node. */
  return buf;
}

/* total size, in bytes */
off_t get_size(struct stat *file_info) {
  return file_info->st_size;
}

/* time of last modification */
time_t get_mod_time(struct stat *file_info) {
  return file_info->st_mtime;
}

/* convert struct time_t to struct tm ==> gives access to local time */
struct tm *format_time(time_t time) {
  struct tm *formatted = malloc(sizeof(struct tm));
  localtime_r(&time, formatted); /* method from lib time.h */
  return formatted;
}
