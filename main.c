#include "file_list.h"

int main(int argc, char **argv) {
    int opt_a = 0, opt_t = 0;
    char **paths = NULL;
    int path_count = 0;

    if (parse_args(argc, argv, &opt_a, &opt_t, &paths, &path_count) != 0) {
        fprintf(stderr, "Error parsing command line arguments.\n");
        free(paths); // free allocated memory before we quit
        return 1; // Exit if there was an error parsing args
    }

    for (int i = 0; i < path_count; i++) {
        if (path_count > 1) {
            printf("%s:\n", paths[i]);
        }
        list_directory(paths[i], opt_a, opt_t);
        if (i < path_count - 1) {
            printf("\n");
        }
    }

    free(paths); // free  allocated array
    return 0;
}