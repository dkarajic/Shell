#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <ctype.h>
#include <string.h>
#include <unistd.h>

static char *history[SUSHI_HISTORY_LENGTH] = {NULL};

char *sushi_read_line(FILE *in) {
    char *input;
    input = (char*) malloc(SUSHI_MAX_INPUT * sizeof(char));
    if (input == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    char *str = fgets(input, SUSHI_MAX_INPUT, in);
    if (str != NULL) {
        if (strlen(str) < SUSHI_MAX_INPUT) {
            fprintf(stderr, "Line too long, truncated");
        }
        int i;
        for (i = 0; i < (int) strlen(str); i++) {
            if (isspace(str[i]) == 0) {
                break;
            }
            else if (i == ((int) strlen(str - 1)) || isspace(str[i]) != 0) {
                return NULL;
            }
        }
        return input;
    }
    else {
        perror("I/O failed");
        return NULL;
    }
    free(input);
}

int sushi_read_config(char *fname) {
    /*
    if (access(fname, F_OK) != 1) {
        FILE *file = fopen(fname, "r");
        if (file != NULL) {
            while(fgets)
            //sushi_read_line
            //store sushi_store
            return 0;
        }
        else {
            perror("Could not open file")
            return 1;
        }
    }
    else {
        return 1;
    }
    */
    return 0;
}
