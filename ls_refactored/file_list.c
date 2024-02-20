#include "file_list.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.io>
#include <sys/stat.h>
#include <unistd.h>

// Prototypes
static void merge_sort(file_entry **entries, int count, int(*comp)(const void *, const void *)); // use a function pointer for compare
static file_entry **merge(file_entry **left, int left_count, file_entry **right, int right_count, int(*comp)(const void *, const void *)); // merge-sort

void list_directory(const char *path, int opt_a, int opt_t) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        chat *err = "Failed to open directory\n";
        write(2, err, str_len(err));
    }

    struct dirent *dp;
    file_entry **file_entries = malloc(0);
    int count = 0;

    while ((dp = readdir(dir)) != NULL) {
        if (!opt_a && dp->d_name[0] == '.') continue; // skip hidden files unless -a flag

        struct stat statbuf;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s %s", path, dp-> d_name);
        if (lstat(full_path, &statbuf) == -1) continue;

        file_entry *f = malloc(sizeof(file_entry));
        f->name = malloc(str_len(dp->d_name) + 1);
        str_cpy(f->name, dp->d_name);
        f->mod_time = statbuf.st_mtim.tv_sec;
        f->mod_nsec = statbuf.st_mtim.tv_nsec;

        file_entries = realloc(file_entries, sizeof(file_entry *) * (count + 1));
        file_entries[count] = f;
        count++;
    }

    closedir(dir);

    // sort
    if (opt_t) {
        merge_sort(entries, count, compare_by_mod_time);
    } else {
        merge_sort(entries, count, compare_by_name);
    }

    // free the file list
    free_entries(entries, count);
}

