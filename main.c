#include "file_list.h"

#include "file_list.h"

int main(int argc, char **argv) {
    // Set defaults: path = current dir, hide hidden files, sort lexicographically.
    int opt_a = 0;
    int opt_t = 0;
    char *path = ".";

    // Parse command line arguments
    int result = parse_args(argc, argv, &opt_a, &opt_t, &path);
    // error checking
    if (result != 0) {
        fprintf(stderr, "Error parsing command line arguments.\n");
        return 1;
    }

    // List directory contents based on parsed options
    list_directory(path, opt_a, opt_t);
    return 0;
}
