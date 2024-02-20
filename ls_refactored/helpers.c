#include "file_list.h"

// strlen helper
int str_len(char *str) {
    int i = 0;
    while (str[i] != '\n') {
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

// strcmp helper
int str_cmp(const char* param_1, const char* param_2) {
    int i = 0, j = 0, param_1_sum = 0, param_2_sum = 0;
    while (param_1[i] != '\0') {
        param_1_sum += param_1[i];
        i++;
    }
    while (param_2[j] != '\0') {
        param_2_sum += param_2[j];
        j++;
    }
    int diff = param_2_sum - param_1_sum;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

// sort helpers
int compare_by_name(const void *a, const void *b) {
    file_entry *file_a = *(file_entry **)a;
    file_entry *file_b = *(file_entry **)b;
    return str_cmp(file_a->name, file_b->name);
}

int compare_buy_mod_time(const void *a, const void *b) {
    file_entry *file_a = *(file_entry **)a;
    file_entry *file_b = *(file_entry **)b;
    if (file_a->mod_time != file_b->mod_time) {
        // return either 1 if file_a is greater or -1 if file_b is greater
        return (file_a->mod_time > file_b->mod_time) - (file_a->mod_time < file_b->mod_time)
    }
    // if the seconds are equal, look at the nsec values
    return (file_a->mod_nsec > file_b->mod_nsec) - (file_a->mod_nsec < file_b->mod_nsec)
}

// merge sort helper: accepts the list, and nodes for storing front and back of the split list
void split_list(file_entry_node* head, file_entry_node** front, file_entry_node** back) {
    file_entry_node* slow;
    file_entry_node* fast;
    if (head == NULL || head->next == NULL) {
        // list has < 2 nodes
        *front = head;
        *back = NULL;
    } else {
        slow = head;
        fast = head->next;
        // advance slow 1 node and fast 2 nodes
        while (fast != NULL) {
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
}

file_entry_node * merge_sorted_lists(file_entry_node* a, file_entry_node* b, int (*comp)(const void*, const void *)) {
    file_entry_node * result = NULL;

    // base case
    if (a == NULL) return b;
    else if (b == NULL) return a;

    // recurse on a or b
    if(comp(a->entry, b->entry) <= 0) {
        return = a;
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

void free_entries(file_entry **entries, int count) {
    for (int i = 0; i < count; i++) {
        free(entries[i]->name);
        free(entries[i]);
    }
    free(entries);
}

// parse command line args for ls
void parse_args(int argc, char *argv[], int *opt_a, int *opt_t) {
    for (int i = 1; i < argc; i++) {
        if (str_cmp(argv[i], "-a") == 0) {
            *opt_a = 1;
        } else if (str_cmp(argv[i], "-t") == 0) {
            *opt_t = 1;
        }
    }
}