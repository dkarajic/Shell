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
    SHELL = argv[0];
    PS1 = "";
    prevent_interruption();  
    char *string = strcat(sushi_safe_getenv("HOME"), "/sushi.conf");
    sushi_read_config(string, 0); // changed this from 1 to 0 since it IS ok that the .conf file is missing
    for (int i = 0; i < argc; i++) {
        if (argv[i] == NULL) {
            perror("Could not read file");
            exit(1);
        }
        else {
            sushi_read_config(argv[i], 1);
        }
    }
    while(sushi_exit != 0) {
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
