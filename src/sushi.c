#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include "sushi_read.c"
#include "sushi_history.c"

int main(int argc, char *argv[]) {
    FILE *file = fopen("test.txt", "r");
    printf("%s", sushi_read_line(file));
    sushi_store("abcd");
    //sushi_read_config("test1.txt");
    return EXIT_SUCCESS;
}

