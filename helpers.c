#include "file_list.h"
#include <stdio.h>

// strlen helper
int str_len(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

//strcpy helper
char* str_cpy(char* dst, char* src) {
    // A pointer to the beginning of the dst string
    char* original_dst = dst;

    // Copy characters from src to dst one by one
    while (*src) {
        *dst = *src;
        dst++;
        src++;
    }

    // Null-terminate the destination string -> does not follow exactly the spec for strcpy.
    *dst = '\0';

    // Return the original pointer to dst
    return original_dst;
}

// strcmp helper - refactored
int str_cmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// sort helpers
int compare_by_name(const void *a, const void *b) {
    // Cast from void pointer to file_entry_node pointer
    const file_entry_node *node_a = a;
    const file_entry_node *node_b = b;

    // the entry field of each node is the struct containing the pathname
    file_entry *file_a = node_a->entry;
    file_entry *file_b = node_b->entry;

    return str_cmp(file_a->name, file_b->name);
}

int compare_by_mod_time(const void *a, const void *b) {
    // Cast from void pointer to file_entry_node pointer
    const file_entry_node *node_a = a;
    const file_entry_node *node_b = b;

    const file_entry *file_a = node_a->entry;
    const file_entry *file_b = node_b->entry;

    // Check modification time in seconds
    if (file_a->mod_time > file_b->mod_time) {
        return -1;  // More recent file first
    } else if (file_a->mod_time < file_b->mod_time) {
        return 1;
    }

    // If modification time in seconds is the same, compare nanoseconds
    if (file_a->mod_nsec > file_b->mod_nsec) {
        return -1;
    } else if (file_a->mod_nsec < file_b->mod_nsec) {
        return 1;
    }

    // If nanoseconds are exactly the same, fallback to lexicographical order
    return strcmp(file_a->name, file_b->name);
}

// merge sort helper: accepts the list, and nodes for storing front and back of the split list
void split_list(file_entry_node* head, file_entry_node** front, file_entry_node** back) {
    file_entry_node* slow;
    file_entry_node* fast;
    slow = head;
    fast = head->next;

    while (fast != NULL) {
        // advance fast 2 nodes and slow 1 node
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    //  split list at midpoint
    *front = head;
    *back = slow->next;
    slow->next = NULL;

}

file_entry_node * merge_sorted_lists(file_entry_node* a, file_entry_node* b, int (*comp)(const void*, const void *)) {
    file_entry_node * result = NULL;

    // base case
    if (a == NULL) return b;
    else if (b == NULL) return a;

    // recurse on a or b
    if(comp(a, b) <= 0) {
        result = a;
        result->next = merge_sorted_lists(a->next, b, comp);
    } else {
        result = b;
        result->next = merge_sorted_lists(a, b->next, comp);
    }
    return result;
}

void merge_sort(file_entry_node** headRef, int(*comp)(const void*, const void*)) {
    file_entry_node* head = *headRef;
    file_entry_node* a;
    file_entry_node* b;

    // base case
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    // split head into sublists
    split_list(head, &a, &b);

    // recursively sort sublists
    merge_sort(&a, comp);
    merge_sort(&b, comp);

    // merge sorted lists
    *headRef = merge_sorted_lists(a, b, comp);
}

// parse cli args
int parse_args(int argc, char *argv[], int *opt_a, int *opt_t, char ***paths, int *path_count) {
    int has_error = 0; // error tracking
    *path_count = 0; // initialize count of dir arguments at 0

    *paths = malloc(argc * sizeof(char*)); // Allocate array for paths based on the number of arguments
    // error check
    if (*paths == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 1; // Memory allocation failure
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {  // Handle flags
            // iterate argv flags and set them
            int j = 1;
            while (argv[i][j]) {
                if (argv[i][j] == 'a') {
                    *opt_a = 1;
                } else if (argv[i][j] == 't') {
                    *opt_t = 1;
                } else {
                    fprintf(stderr, "Unknown option %s\n", argv[i]);
                    has_error = 1;  // Mark error but continue parsing
                }
                j += 1;
            }
        } else {
            (*paths)[*path_count] = argv[i];
            (*path_count)++;
        }
    }

    // handle default path - no user path input
    if (*path_count == 0) {
        (*paths)[*path_count] = ".";
        (*path_count)++;
    }

    return has_error; // return error status, 0 on success
}

// current directory (.) then root directory ('/') should always be listed first
void sort_paths(char **paths, int path_count) {
    for (int i = 0; i < path_count; i++) {
        if (str_cmp(paths[i], ".") == 0) {
            // Move current directory (.) to the beginning
            for (int j = i; j > 0; j--) {
                char *temp = paths[j];
                paths[j] = paths[j-1];
                paths[j-1] = temp;
            }
        }
    }
    for (int i = 0; i < path_count; i++) {
        if (str_cmp(paths[i], "/") == 0) {
            // Move root directory (/) to the second position
            for (int j = i; j > 1; j--) {
                char *temp = paths[j];
                paths[j] = paths[j-1];
                paths[j-1] = temp;
            }
        }
    }
}


// helper function to debug printing file mod times
// void print_file_times(const char *filename) {
//     struct stat statbuf;
//     if (lstat(filename, &statbuf) == -1) {
//         perror("Failed to get file stats");
//         return;
//     }
//     printf("File: %s, Time: %ld.%ld\n", filename, MOD_TIME_SEC(statbuf), MOD_TIME_NSEC(statbuf));
// }
