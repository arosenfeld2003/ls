#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// COMPUTATIONAL THINKING (Peter J Denning) (Get the book)

// Linked List
struct List {
  char data;
  struct List *next;
};

// node
struct Node {};

create_list();  // ==> returns pointer to List; (no data)
                // ==> data = NULL; next = NULL;
destroy_list();


int main() {
  struct my_list *list;
  struct node *node;
  list = create_list();
  node = create_node(data);
  list = node_add_to_list(list,node);
  print_list(list);
  destroy_list(list);
}

// typedef ==> t_


// void sort_list(t_list **, int sort_func(void *));

// compare_size(nodeA, nodeB)
// compares size, returns integer: 1 (bigger), 0(equal), -1(less)

// TODO:
// 1. if not a directory, print filename ==> done, 1/31 (NOT A LIST).
// 2. add necessary stat info to the void pointer data for each list node.
// 3. SORT LIST FUNCTIONS - OPTIONS ==>
  // sort alphabetically ==> DONE, 1/30.
  // create a way to accept options. ==> DONE, 2/1.
  // -a : include files starting with a "." ==> DONE, 2/1
    // (if no flag, removes hidden files from list)
  // -t : sort/display time modified. ==> in progress(2/1).
    // malloc error?!

    /*
      my_ls(62252,0x116061dc0) malloc: Incorrect checksum for freed object 0x7fdbad401a78: probably modified after being freed.
      Corrupt value: 0x0
      my_ls(62252,0x116061dc0) malloc: *** set a breakpoint in malloc_error_break to debug
      Abort trap: 6
     */
    
    // sorting works.
    // segfault when trying to print.
  // -z : display number of bytes in each file.
  // -R : recursively list sub-directories.


  /*
void my_putc(char);

int main () {
  void(*F)(char c);
  F = my_putc;
  F('B');
  F('\n');

  return 0;
}

void my_putc(char c) {
  write(1, &c, 1);
}
*/