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


void sort_list(t_list **, int sort_func(void *));

compare_size(nodeA, nodeB)
// compares size, returns integer: 1 (bigger), 0(equal), -1(less)