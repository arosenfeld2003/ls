#include "file_list.h"

void list_directory(const char *path, int opt_a, int opt_t) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        char *err = "Failed to open directory\n";
        write(2, err, str_len(err));
    }

    struct dirent *dp;
    file_entry_node *head = NULL; // head of our linked_list for files
    file_entry_node *current = NULL; // current node

    while ((dp = readdir(dir)) != NULL) {
        if (!opt_a && dp->d_name[0] == '.') continue; // skip hidden files unless -a flag

        struct stat statbuf;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, dp-> d_name);
        if (lstat(full_path, &statbuf) == -1) continue;

        file_entry *f = malloc(sizeof(file_entry));
        f->name = malloc(str_len(dp->d_name));
        str_cpy(f->name, dp->d_name);
        f->mod_time = MOD_TIME_SEC(statbuf); // seconds
        f->mod_nsec = MOD_TIME_NSEC(statbuf); // nanoseconds

        file_entry_node *new_node = malloc(sizeof(file_entry_node));
        new_node->entry = f;
        new_node->next = NULL;

        // update the list
        if (head == NULL) {
            head = new_node;
            current = new_node;
        } else {
            current->next = new_node;
            current = new_node;
        }
    }

    closedir(dir);

    // sort
    if (head != NULL) {
        if (opt_t) {
            merge_sort(&head, compare_by_mod_time);
        } else {
            merge_sort(&head, compare_by_name);
        }
    }

    // print file names
    file_entry_node *temp;
    while (head != NULL) {
        printf("%s\n", head->entry->name);
        temp = head;
        head = head->next;

        // free the node after printing
        free(temp->entry->name);
        free(temp->entry);
        free(temp);
    }
}

