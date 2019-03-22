#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <signal.h>
#include <string.h>

int sushi_exit = 0;

static void refuse_to_die(int sig)
{
    fprintf("Type exit to exit the shell\n")
    struct sigaction action;
    sigaction(sig, &action, NULL);
}

static void prevent_interruption() {
    signal(SIGINT, refuse_to_die);
    //fprintf(stderr, "Type exit to exit to the shell\n");
}

int main(/*int argc, char *argv[]*/) {
    prevent_interruption();  
    sushi_read_config(strcat(sushi_safe_getenv("HOME"), "/sushi.conf"));
    while(sushi_exit != 0) {
        printf("%s", SUSHI_DEFAULT_PROMPT);
        char *line = sushi_read_line(stdin);
        if (line == NULL) {
            continue;
        }
        else {
            if(sushi_parse_command(line) != 0) {
                sushi_store(line); 
            }
        }
        //sushi_show_history();
    }  
    return EXIT_SUCCESS; 
}
