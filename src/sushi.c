#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

//int argc, char *argv[]
int main(void) {
    if (sushi_read_config("sushi.conf") == 1) {
        return EXIT_FAILURE;
    }
    printf("%s", SUSHI_DEFAULT_PROMPT);
    if (sushi_read_line(stdin) == NULL) {
        return EXIT_FAILURE;
    }
    sushi_show_history();
    return EXIT_SUCCESS;
}
