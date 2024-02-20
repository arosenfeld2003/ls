#include "file_list.h"
#include <stdio.h>

int main(int argc, char **argv) {
    int opt_a = 0;
    int opt_t = 0;

    parse_args(argc, argv, &opt_a, &opt_t);
    list_directory(".", opt_a, opt_t);

    return 0;
}