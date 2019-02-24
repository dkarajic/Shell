#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

int sushi_exit = 0;

static void refuse_to_die(int sig)
{
  // TODO
}

static void prevent_interruption() {
  // TODO
}

int main(/*int argc, char *argv[]*/) {
  prevent_interruption();  
    while(sushi_exit != 0) {
        if (sushi_read_config("$HOME/sushi.conf") == 1) {
            return EXIT_FAILURE;
        }
        printf("%s", SUSHI_DEFAULT_PROMPT);
        if (sushi_read_line(stdin) == NULL) {
            return EXIT_FAILURE;
        }
        else {
            if(sushi_parse_command(sushi_read_line(stdin)) != 0) {
                sushi_store(sushi_read_line(stdin));               
            }
        }
        sushi_show_history();
        return EXIT_SUCCESS;   
    }
}
