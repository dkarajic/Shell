#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sushi_yyparser.tab.h"

// https://en.wikipedia.org/wiki/Escape_sequences_in_C#Table_of_escape_sequences 
char *sushi_unquote(char * s) {
    int i, j;
    for(i = 0, j = 0; i < (int) strlen(s) - 1; i++, j++) {
        if (s[i] == '\\') {
            switch(s[i + 1]) {
                case 'a':
                    s[j] = '\a';
                    i++;
                    break;
                case 'b':
                    s[j] = '\b';
                    i++;
                    break;
                case 'f':
                    s[j] = '\f';
                    i++;
                    break;
                case 'n':
                    s[j] = '\n';
                    i++;
                    break;
                case 'r':
                    s[j] = '\r';
                    i++;
                    break;
                case 't':
                    s[j] = '\t';
                    i++;
                    break;
                case 'v':
                    s[j] = '\v';
                    i++;
                    break; 
                case '\\':
                    s[j] = '\\';
                    i++;
                    break;
                case '\'':
                    s[j] = '\'';
                    i++;
                    break;
                case '\"':
                    s[j] = '\"';
                    i++;
                    break;
                case '?':
                    s[j] = '?';
                    i++;
                    break;
            }
        }
        else {
            s[j] = s[i];
        }
    }
    s[j] = '\0';
    return s;
}

/*
// Alternate method
char *sushi_unquote(char * s) {
    for(int i = 0; i < (int) strlen(s) - 1; i++) {
        if (s[i] == '\\') {
            switch(s[i + 1]) {
                case 'a':
                    s[i] = '\a';
                    break;
                case 'b':
                    s[i] = '\b';
                    break;
                case 'f':
                    s[i] = '\f';
                    break;
                case 'n':
                    s[i] = '\n';
                    break;
                case 'r':
                    s[i] = '\r';
                    break;
                case 't':
                    s[i] = '\t';
                    break;
                case 'v':
                    s[i] = '\v';
                    break; 
                case '\\':
                    s[i] = '\\';
                    break;
                case '\'':
                    s[i] = '\'';
                    break;
                case '\"':
                    s[i] = '\"';
                    break;
                case '?':
                    s[i] = '?';
                    break;
            }
            for (int j = i + 1; j < (int) strlen(s) - 1; j++) {
                s[j] = s[j + 1];
            }
            s[(int) strlen(s) - 1] = '\0';
        }
    }
    return s;
}
*/

// Do not modify this function
void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
