#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

void sushi_store(char *line) { 
    if (line != NULL) {
        if((history[SUSHI_HISTORY_LENGTH-1]) != NULL) {
            free(history[SUSHI_HISTORY_LENGTH-1]);
        }
        for (int i = SUSHI_HISTORY_LENGTH-1; i > 0; i--) {
            bcopy(history[i-1], history[1], 1);
        }
        history[0] = line;
    }    
}

void sushi_show_history() {
    for (int j = 0; j < SUSHI_HISTORY_LENGTH; j++) {
        if (history[j] != NULL) {
            //printf("%d  %s\n", j+1, history[j]);
        }
    }
}
