#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <signal.h>
#include <string.h>

int sushi_exit = 0;
char *SHELL;
char *PS1;

static void refuse_to_die(int sig)
{
    fprintf(stderr, "Type exit to exit the shell\n");
    struct sigaction action;
    sigaction(sig, &action, NULL);
}

static void prevent_interruption() {
    signal(SIGINT, refuse_to_die);
}

int main(int argc, char *argv[]) {
  // DZ: Why haven't you fixed these?
    SHELL = argv[0];    
    PS1 = "";
    prevent_interruption();
    // DZ: Should not concatenate a string of unknown length and another string
    char *string = strcat(sushi_safe_getenv("HOME"), "/sushi.conf");
    sushi_read_config(string, 0); // changed this from 1 to 0 since it IS ok that the .conf file is missing
    // DZ: From 1, not from 0
    for (int i = 1; i < argc; i++) {
      // DZ: This won't be possible.
        if (argv[i] == NULL) {
            perror("Could not read file");
            exit(1);
        }
        else {
            sushi_read_config(argv[i], 1);
        }
    }

    // DZ: Wrong condition check!
    while(sushi_exit != 1) {
        if (PS1) {
            printf("%s", PS1);
        }
        else {
            printf("%s", SUSHI_DEFAULT_PROMPT);
        }
        char *line = sushi_read_line(stdin);
        if (line == NULL) {
            continue;
        }
        else {
            if(sushi_parse_command(line) != 0) {
                sushi_store(line); 
            }
        }
    }  
    return EXIT_SUCCESS; 
}
