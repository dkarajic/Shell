#include <sys/types.h>
#include <sys/wait.h>
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
    // s[j] = '\0';
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
void free_memory(prog_t *exe) {
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
    
    //free command line
    free_memory(exe->prev);
}

// Skeleton
void sushi_assign(char *name, char *value) {
    if (setenv(name, value, 1) == 0) {
        free(name);
        free(value);
    }
    else {
        perror(setenv(name, value, 1));
    }
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

/*------------------------------------------------------------------
 * You can use these "convenience" functions as building blocks for
 * HW5 instead of your code, if you want. You may change them, too.
 *------------------------------------------------------------------*/

// Find the number of programs on the command line
static size_t cmd_length(prog_t *exe) {
  int count = 0;
  while(exe->prev) {
    exe = exe->prev;
    count++;
  }
  return count;
}

// Wait for the process pid to terminate; once it does, set the
// environmental variable "_" to the exit code of the process.
static int wait_and_setenv(pid_t pid) {
  int status;
  if (-1 == waitpid(pid, &status, 0)) {
    perror("waitpid");
    status = 1; // Something bad happened
  }
  char retval[16]; // Just to be safe
  sprintf(retval, "%d", status);
  if(-1 == setenv("_", retval, 1)) {
    perror("_");
    return 1;
  } else
    return 0;
}

// Execute the program defined in "exe"
static void start(prog_t *exe) {
  arglist_t args = exe->args;
  args.args = realloc(args.args, sizeof(char*) * (args.size + 1));
  args.args[args.size] = (char*)NULL;
  execvp(args.args[0], args.args);
  perror(args.args[0]);
}

// "Rename" fule descriptor "old" to "new," if necessary. After the
// execution of this function a program that "believes" that it uses
// the "old" descriptor (e.g., stdout #1 for output) will be actually
// using the "new" descriprot (e.g., an outgoinf pipe).  This
// functions terminates the process of error and should not be used in
// the parent, only in a child.
static void dup_me (int new, int old) {
  if (new != old && -1 == dup2(new, old)) {
    perror("dup2");
    exit(1);
  }
}

/*
 * You can use this function instead of yours if you want.
 */
int sushi_spawn_dz(prog_t *exe, int bgmode) {
  int pipe_length = 0, max_pipe = cmd_length(exe);
  pid_t pid[max_pipe];

  int old_stdout = STDOUT_FILENO;
  
  for(prog_t *prog = exe; prog; prog = prog->prev) {
    int pipefd[2] = {STDIN_FILENO, old_stdout};
    if (prog->prev && -1 == pipe(pipefd)) {
      perror("pipe");
      return 1;
    }
  
    switch(pid[pipe_length] = fork()) {
    case -1: // Error
      perror(prog->args.args[0]);
      return 1;
    case 0: // Child
      dup_me(pipefd[0], STDIN_FILENO);
      dup_me(old_stdout, STDOUT_FILENO);
      if(pipefd[1] != STDOUT_FILENO)
	close(pipefd[1]);
      start(prog);
      exit(1);
    default: // Parent
      if(pipefd[0] != STDIN_FILENO) close(pipefd[0]);
      if(old_stdout != STDOUT_FILENO) close(old_stdout);
      old_stdout = pipefd[1];
    }
    pipe_length++;
  }

  int status = 0;
  if (bgmode == 0) {
    for (int i = 0; i < pipe_length; i++)
      if(wait_and_setenv(pid[i]))
	status = 1;
  }

  if(old_stdout != STDOUT_FILENO) close(old_stdout);
  free_memory(exe);
  
  return status;
}

/*--------------------------------------------------------------------
 * End of "convenience" functions
 *--------------------------------------------------------------------*/

int sushi_spawn(prog_t *exe, int bgmode) {
    // singlular solution
    int id_array[2];
    int pfd[2];
    int pid = fork();
    int status;
    if (pid == 0) {
        // child
        pipe[pfd];
        int pid2 = fork();
        if (pid2 == 0) {
            // grandchild
            grandchild_id = getpid();
            id_array[0] = grandchild_id;
            close(pfd[0]);
            dup2(pfd[1], 1);
            execvp(grandchild_id, pfd);
        }
        if (pid2 > 0) {
            // original child
            child_id = getpid();
            id_array[1] = child_id;
            close(pfd[1]);
            dup2(pfd[0], 0);
            execvp(child_id, pfd)
        }
        exe->args.args = super_realloc(exe->args.args, (exe->args.size + 1) * sizeof (char*));
        exe->args.args[exe->args.size] = NULL;
        if (execvp(exe->args.args[0], exe->args.args) == -1) {
            perror(execvp(exe->args.args[0]));
            exit(1);
            }
        }
        if (pid > 0) {
            // parent
            if (bgmode == 1) {
                free_memory(exe);
                return 0;
            }
            else {
                // bgmode == 0
                free_memory(exe);
                if (waitpid(pid, &status, 0) != 1); {
                    char string[sizeof(char)];
                    sprintf(string, "%d", status);
                    setenv("_", string, 1);
                    return 0;
                }
                else {
                    perror(waitpid(pid, &status, 0));
                }
            }
        }
        else {
            // failure
            perror("Fork failed");
            return 1;
        }
        exe = exe->prev;
        i++;
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

/*
 * New skeleton functions
 */
void sushi_display_wd() {
}

void sushi_change_wd(char *new_wd) {
}
