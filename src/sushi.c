#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include "sushi_read.c"
#include "sushi_history.c"

int main(int argc, char *argv[]) {
    //sushi_read_config("test.txt");
    char *text1;
    char *text2;
    char *text3;
    text1 = "sample text";
    text2 = "more of it";
    text3 = "and then some";
    sushi_store(text1);
    sushi_store(text2);
    sushi_store(text3);
    //sushi_show_history();
    return EXIT_SUCCESS;
}