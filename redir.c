#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define INPUT_MAX 100
#define ARG_MAX 10

int main(int argc, char* argv[]) {
    
    if (argc < 3) {
        fprintf(stderr,
            "Usage: %s <inputfile> <command> <outputfile>\n", argv[0]);
        return 1;
    }

    int fd_in;
    int fd_out;

    if (*argv[0] == '-'){
        
        fd_in = STDIN_FILENO;

    }
    if (*argv[3] == '-'){

        fd_in = STDOUT_FILENO;

    }
    if (*argv[3] != '-'){
        
        int fd_in = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd_in == -1) {
            fprintf(stderr, "Failed to open %s\n", argv[3]);
            return 1;
        }
    }
    if(*argv[0] != '-'){

        fd_out = open(argv[1], O_RDONLY);
        if (fd_out == -1) {
        fprintf(stderr, "Failed to open %s\n", argv[1]);
        return 1;
        }

    }

    char *newargv[ARG_MAX];
    int argv_size = 0;

    char *token = strtok(argv[2], " ");

    while (token != NULL && argv_size < ARG_MAX){
        newargv[argv_size] = malloc(strlen(token) + 1);
        strcpy(newargv[argv_size], token);
        argv_size++;
    }

    newargv[argv_size] = NULL;
    argv_size++;

    int child_pid = fork();
    if (child_pid == 0) {
        dup2(fd_in, STDIN_FILENO); 
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);

        execve("/usr/bin/", newargv, NULL);
    }

    wait(NULL);
    printf("%s pid is %d. forked %d. "
           "Parent exiting\n",
        argv[0], getpid(), child_pid);

    return 0;
}
