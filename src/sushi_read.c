#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <ctype.h>
#include <string.h>
#include <unistd.h>

char *sushi_read_line(FILE *in) {
    char *input;
    input = (char*) malloc(SUSHI_MAX_INPUT);
    if (input == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    fgets(input, SUSHI_MAX_INPUT, in);
    if (input != NULL) {
        if (input[(strlen(input)-1)] != '\n') {
            fprintf(stderr, "Line too long, truncated\n");
        }
        else {
            input[strlen(input) - 1] = '\0';
        }
        for (int i = 0; i < (int) strlen(input); i++) {
            if (isspace(input[i]) == 0) {
                break;
            }
            else if (i == ((int) strlen(input - 1)) && isspace(input[i]) != 0) {
                return NULL;
            }
        }
        return input;
    }
    else {
        perror("I/O failed");
        return NULL;
    }
}

int sushi_read_config(char *fname) {
    if (access(fname, F_OK) != 1) {
        FILE *file = fopen(fname, "r");
        if (file != NULL) {
            do {
                sushi_store(sushi_read_line(file));
            } while((!feof(file)));
            return 0;
        }
        else {
            perror("Could not open file");
            return 1;
        }
        fclose(file);
    }
    else {
        return 1;
    }
}
