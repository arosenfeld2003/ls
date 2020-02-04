/*
**
*/

#include "./my_list.h"

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

// testing
int main(int argc, char **argv) {
  char *filename = argv[1];
  struct stat *buf = malloc(sizeof(stat));
  stat(filename, buf);

  // printf("%lld\n", get_size(buf));

  // time_t modtime = get_mod_time(buf);
  // printf("%ld\n", modtime);

  // struct tm *formatted_time = format_time(modtime);
  // printf("Month: %d\n", formatted_time->tm_mon);
  // printf("Day of month: %d\n", formatted_time->tm_mday);
  // printf("Day of week: %d\n", formatted_time->tm_wday);
  // printf("Year: %d\n", (formatted_time->tm_year) + 1900);
  // printf("Hours: %d\n", formatted_time->tm_hour);
  // printf("Minutes: %d\n", formatted_time->tm_min);
  // printf("Seconds: %d\n", formatted_time->tm_sec);



  // free(formatted_time);

  free(buf);
  return 0;
}