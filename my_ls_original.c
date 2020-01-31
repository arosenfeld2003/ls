#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/*
    -a Include directory entries whose names begin with a dot (.).
      sruct dirent.d_name

    -z the number of bytes in the file is displayed for each file.

    -R Recursively list subdirectories encountered.

    -t Sort by time modified (most recently modified first)...
       before sorting the operands by lexicographical order.
*/


// Re-think:
// DATA STRUCTURES.
// Create a Linked List.
// Needs to be sorted.

// DIR *folder;
// struct dirent *entry;
// struct stat filestat;

// struct stat {
//     dev_t     st_dev;     /* ID of device containing file */
//     ino_t     st_ino;     /* inode number */
//     mode_t    st_mode;    /* protection */
//     nlink_t   st_nlink;   /* number of hard links */
//     uid_t     st_uid;     /* user ID of owner */
//     gid_t     st_gid;     /* group ID of owner */
//     dev_t     st_rdev;    /* device ID (if special file) */
//     off_t     st_size;    /* total size, in bytes */
//     blksize_t st_blksize; /* blocksize for file system I/O */
//     blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
//     time_t    st_atime;   /* time of last access */
//     time_t    st_mtime;   /* time of last modification */
//     time_t    st_ctime;   /* time of last status change */
// };

// POSIX macros are defined to check the file type using the st_mode field:
//     S_ISREG(m)
//     is it a regular file?
//     S_ISDIR(m)
//     directory?

// opendir(dir) ==> Returns a pointer to head of directory stream (DIR *dirp).
// readdir(dirp) ==> Returns a pointer to a dirent structure...
//                   representing the next directory entry...
//                   in the directory stream pointed to by dirp.
//                   Returns NULL on reaching end of the directory stream or on error.

// struct dirent {
//   ino_t  d_ino; // Inode number.
//   char   d_name[256]; // Null-terminated filename.
// }

/*
1/22: TODO
1. Create linked list of files.
2. Handle options: -a, -z, -R, -t.
  CREATE OPTIONS STRUCT.
3. Sort the order of printing.
4. Handle spacing.
*/

// define Options struct.
typedef struct Options {
  int include_hidden_files; // -a
  int show_size_in_bytes; // -z
  int list_dirs_recursively; // -R
  int sort_by_time_modified; // -t
} t_opts;

// initialize without any options.
t_opts init_opts(struct Options *opts) {
  opts->include_hidden_files = 0;
  opts->show_size_in_bytes = 0;
  opts->list_dirs_recursively = 0;
  opts->sort_by_time_modified = 0;
  return *opts;
}

// define Node struct.
typedef struct Node {
  char *name;
  struct stat *info;
  struct Node *next;
} Node;

// create a Node for a given filename, containing stat info.
struct Node *create_node(char *filename) {
  struct Node *new_node = malloc(sizeof(Node)); /* malloc new node. */
  struct stat *buf = malloc(sizeof(stat));      /* malloc a buffer for the stat. */
  stat(filename, buf); /* fill stat buffer with stat of node. */
  new_node->name = filename; /* name the node */
  new_node->info = buf; /* assign the stat to the info property of the new node. */
  new_node->next = NULL; /* assign the next property of new node as NULL; */
  free(buf); /* free the buffer memory. */
  return new_node; /* return the new node. */
}

// add a node to the end of list.
struct Node *add_node(struct Node *head, struct Node *new_node) {
  struct Node *temp = head; /* create pointer to traverse */
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = new_node; /* point 'next' to the new node. */
  return head;
}

// print file names that aren't hidden.
void print_unhidden_file(char *name) {
  if (name[0] != '.') {
    printf("%s\n", name);
  }
}

// print all files
void print_every_file(char *name) {
  printf("%s\n", name);
}

// read directory and print each filename.
void print_dir(DIR *dirp, struct Options *opts) {
  struct dirent *file;
  while ((file = readdir(dirp)) != NULL) {
    char *name = file->d_name;
    opts->include_hidden_files == 0 ? print_unhidden_file(name) : print_every_file(name);
  }
}

// traverse linked list and print file names.
void print_files(struct Node *head, struct Options *opts) {
  while (head != NULL) {
    char *name = head->name;
    opts->include_hidden_files == 0 ? print_unhidden_file(name) : print_every_file(name);
    head = head->next;
  }
}

// read the args from stdin.
int main(int argc, char **argv) {

  // set defaults to no options.
  struct Options *opts;
  init_opts(opts);

  int i = 1;
  // handle flags
  if (argv[i][0] == '-') {
    switch(argv[i][1]) {
      case 'a':
        opts->include_hidden_files = 1;
        break;
    }
    i++;
  }

  // if no files passed in.
  if (argc == 1) {
    DIR *dirp = opendir(".");
    print_dir(dirp, opts);
    closedir(dirp);
    return 0;
  }

  for (; i < argc; i++) {
    char *filename = argv[i];
    // create a node (head of linked list).
    struct Node *buf = create_node(filename);
    if ((S_ISDIR(buf->info->st_mode))) {
      DIR *dirp = opendir(filename);
      struct dirent *file;
      while ((file = readdir(dirp)) != NULL) {
        char *name = file->d_name;
        // create new node for file within directory.
        struct Node *new_node = create_node(name);
        // add node to end of linked list.
        buf = add_node(buf, new_node);
        push(buf, new_node);
        closedir(dirp);
      }
    }
    print_files(buf, opts);
  }
  return 0;
}