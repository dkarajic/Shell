#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

static char *history[SUSHI_HISTORY_LENGTH] = {NULL};

void sushi_store(char *line) { 
    if (line != NULL) {
        if((history[SUSHI_HISTORY_LENGTH-1]) != NULL) {
            free(history[SUSHI_HISTORY_LENGTH-1]);
        }
        for (int i = SUSHI_HISTORY_LENGTH-1; i > 0; i--) {
            history[i] = history[i-1];
        }
        history[0] = line;
    }    
}

void sushi_show_history() {
    for (int j = 0; j < SUSHI_HISTORY_LENGTH; j++) {
        if (history[j] != NULL) {
            printf("%0.5d  %s\n", j+1, history[j]);
        }
    }
}
