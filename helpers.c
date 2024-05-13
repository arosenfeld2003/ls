#include "file_list.h"

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

    // the entry field of each node is the struct containing the pathname
    file_entry *file_a = node_a->entry;
    file_entry *file_b = node_b->entry;

    // check seconds first
    if (file_a->mod_time != file_b->mod_time)
        return file_a->mod_time - file_b->mod_time;
    return file_a->mod_nsec - file_b->mod_nsec; // compare nanoseconds
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

// parse command line args for ls
void parse_args(int argc, char *argv[], int *opt_a, int *opt_t, char **path) {
    for (int i = 1; i < argc; i++) {
        if (str_cmp(argv[i], "-a") == 0) {
            *opt_a = 1;
        } else if (str_cmp(argv[i], "-t") == 0) {
            *opt_t = 1;
        } else {
            // non-optional argument will be treated as a path
            *path = argv[i];
        }
    }
}