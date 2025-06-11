#include "../include/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void do_the_stuff(int argc, char **argv) {

  if (!db_dir_exist()) {
    fprintf(stderr, "where is db dir\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++) {

    char *arg_copy = strdup(argv[i]);
    int n_commas = count_commas(arg_copy, ',');

    handle_the_situation(n_commas, arg_copy);
  }
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    return EXIT_SUCCESS;
  } else {
    do_the_stuff(argc, argv);
  }
}
