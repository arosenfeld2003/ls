#include "file_list.h"

int main(int argc, char **argv) {
    int opt_a = 0;
    int opt_t = 0;
    char *path = ".";

    parse_args(argc, argv, &opt_a, &opt_t, &path);
    list_directory(path, opt_a, opt_t);

    return 0;
}