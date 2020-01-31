#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// define Options struct.
typedef struct Options {
  int a; // include hidden files.
  int z; // show size in bytes.
  int R; // list dirs recursively.
  int t; // sort by time modified.
} Options;

// initialize without any options.
struct Options *init_opts() {
  struct Options *opts;
  opts->a = 0;
  opts->z = 0;
  opts->R = 0;
  opts->t = 0;
  return opts;
}

// handle options
struct Options *set_options(Options *opts, char *flags) {
  if (flags[0] == '-') {
    int i = 1;
    while (flags[i]) {
      char flag = flags[i];
      switch(flag) {
        case 'a': opts->a = 1; break;
        case 'z': opts->z = 1; break;
        case 'R': opts->R = 1; break;
        case 't': opts->t = 1; break;
      }
      i++;
    }
  }
  return opts;
}

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

// read directory and print each filename.
// struct Node *create_linked_list(DIR *dirp, Node *fileNode) {

//   // create head of list.
//   Node *head = fileNode;
//   Node *current = head;
//   Node *temp;
//   struct dirent *file;
//   char *name;

//   // readdir(dirp) ==> Returns a pointer to a dirent structure of the next dir entry.
//   while ((file = readdir(dirp)) != NULL) {
//     name = file->d_name;

//     // if the name is the same, we are on the head of the list.
//     if (name == fileNode->name) {
//       break;
//     }

//     // add directory entry to the temp node.
//     temp = create_node(name);

//     // traverse the list.
//     while (current->next != NULL) {
//       current = current->next;
//     }

//     // add new node to end of list.
//     current->next = temp;

//     // reset pointer to head of list.
//     current = head;
//   }
//   return head;
// }

int main(int argc, char **argv) {
  Options *opts = init_opts(); /* initialize with no options */

  Node *linked_list = NULL; /* create null Node for linked list */

  if (argc > 1) {
    // set options.
    if (argv[1][0] == '-') {
      char *flags = argv[1];
      opts = set_options(opts, flags);
    }

    int i = 1;
    char *arg = argv[i];
    if (argv[i][0] == '-') {
      // skip options for now.
      i++;
    }

    while (argv[i]) {
      char *file = argv[1];

      Node *fileNode = create_node(file);

      if (linked_list == NULL) {
        // if first node, start linkedList.
        linked_list = fileNode;
      }

      DIR *dirp = opendir(file);

      // readdir(dirp) ==> Returns a pointer to a dirent structure of the next dir entry.
      while ((file = readdir(dirp)) != NULL) {

      }

      closedir(dirp);
      i++;
    }
  } else {
    // no files or flags... list files in current dir.
    char *current = ".";
    Node *fileNode = create_node(current);
    DIR *dirp = opendir(current);
    // Node *linked_list = create_linked_list(dirp, fileNode);
    closedir(dirp);
  }

  while (linked_list->next != NULL) {
    printf("%s\n", linked_list->name);
    linked_list = linked_list->next;
  }

  printf("%s\n", linked_list->name);

  return 0;
}

