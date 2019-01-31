#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

void sushi_store(char *line) {
    static char *array;
    array = (char*) malloc(SUSHI_HISTORY_LENGTH * sizeof(char));
    if (line != NULL) {
        if(sizeof(array) + strlen(line) > SUSHI_HISTORY_LENGTH) {
            free(array + strlen(line));
        }
        else {
            memmove(array + strlen(line), array, strlen(line)*sizeof(char));
            strcpy(array, line);
        }
    }
}

void sushi_show_history() {
}
