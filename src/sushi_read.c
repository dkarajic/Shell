#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <ctype.h>
#include <string.h>
#include <unistd.h>

char *sushi_read_line(FILE *in) {
    char input[SUSHI_MAX_INPUT]; // create static buffer
    char *str = fgets(input, SUSHI_MAX_INPUT, in); // read into it
    if (str != NULL) {
        if (input[(strlen(input)-1)] != '\n') { // measure the length
            strncpy(input, input, SUSHI_MAX_INPUT);
            fprintf(stderr, "Line too long, truncated\n");
        }
        else {
            input[strlen(input) - 1] = '\0';
        }
        for (int i = 0; i < (int) strlen(input); i++) {
            if (isspace(input[i]) == 0) {
                break;
            }
            else if (i == ((int) strlen(input) - 1) && isspace(input[i]) != 0) {
                return NULL;
            }
        }
        char *real_buffer = (char*) super_malloc(SUSHI_MAX_INPUT + 1); // allocate a real buffer
        if (real_buffer == NULL) {
            perror("Memory allocation failed");
            return NULL;
        }
        else {
            strcpy(real_buffer, input); // copy the string
            return real_buffer;
        }
    }
    else {
        perror("I/O failed");
        return NULL;
    }
}

int sushi_read_config(char *fname, int ok_if_missing) {
    if (ok_if_missing != 0) {
        if (access(fname, F_OK) != 0) {
            FILE *file = fopen(fname, "r");
            if (file != NULL) {
                do {
                    if(sushi_parse_command(sushi_read_line(file)) != 0) {
                        sushi_store(sushi_read_line(file));
                    }
                } while((!feof(file)));
                fclose(file);
                return 0;
            }
            else {
                return 1;
            }
        }
        else {
            return 1;
        }
    }
    else {
        sushi_exit = 1;
    }
}
