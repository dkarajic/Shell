#include <string.h>
#include "sushi.h"
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
                default:
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

// Do not modify these functions
void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

void __not_implemented__() {  
    fputs("This operation is not implemented yet\n", stderr);
}

// Function skeletons for HW3
void free_memory(prog_t *exe, prog_t *pipe) {
    //free non-NULL arguments
    for (int i = 0; i < exe->args.size; i++) {
        if (exe->args.args[i] != NULL) {
            free(exe->args.args[i]);
        }
    }
    
    //free array
    free(exe->args.args);
    
    //free non-NULL redirections
    if (exe->redirection.in != NULL) {
        free(exe->redirection.in);
    }
    if (exe->redirection.out1 != NULL) {
        free(exe->redirection.out1);
    }
    if (exe->redirection.out2 != NULL) {
        free(exe->redirection.out2);
    }
    
    //free exe
    free(exe);
}

// Skeleton
void sushi_assign(char *name, char *value) {
    setenv(name, value, 1);
    free(name);
    free(value);
}

// Skeleton
char *sushi_safe_getenv(char *name) {
    char *var = getenv(name);
    if (var ==  NULL) {
        return "";
    }
    else {
        return var;
    }
}

int sushi_spawn(prog_t *exe, prog_t *pipe, int bgmode) {
    int pid = fork();
    int status;
    if (pid == 0) {
        // child
        exe->args.args = super_realloc(exe->args.args, (exe->args.size + 1) * sizeof (char*));
        exe->args.args[exe->args.size] = NULL;
        if (execvp(exe->args.args[0], exe->args.args) == -1) {
            perror("Error");
            exit(0);
        }
    }
    if (pid > 0) {
        // parent
        if (bgmode == 1) {
            free_memory(exe, pipe);
            printf("Memory freed");
            return 0;
        }
        else {
            // bgmode == 0
            free_memory(exe, pipe);
            waitpid(pid, &status, 0);
            char string[sizeof(int)];
            sprintf(string, "%d", status);
            setenv("_", string, 1);
            return 0;
        }
    }
    else {
        // failure
        perror("Fork failed");
        return 1;
    }
}

void *super_malloc(size_t size) {
    void* new_ptr = malloc(size);
    if (new_ptr != NULL) {
        return new_ptr;
    }
    else {
        abort();
    }
}

void *super_realloc(void *ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);
    if (new_ptr != NULL) {
        return new_ptr;
    }
    else {
        abort();
    }
}

char *super_strdup(const char *s) {
    char* new_ptr = strdup(s);
    if (new_ptr != NULL) {
        return new_ptr;
    }
    else {
        abort();
    }
}
