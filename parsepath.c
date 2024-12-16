#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ctype.h>
#include "redir_func.h" 

#define MAX_CMD_INPUT 100
#define MAX_DIR_LENGTH 100

/*
    ./parsepath <inputfile> <command> <outputfile>

    inputfile: file containing information subject to command
    command: one or more linux commands
    outputfile: destination of command results

    Will modify data found in the input file according to the given
        command and output the result into the outputfile
*/

int main(int argc, char* argv[]){

    char* words[MAX_CMD_INPUT];
    int number_of_words = 0;
    char absolue_path[1000];

    // "wc -l" -> "/usr/bin/wc"
    // "wc -l" -> "wc" "-l"
    break_into_words(argv[1], words, &number_of_words, ' ');

    // accounts for number of arguments + NULL
    int number_of_arguments = number_of_words - 1;
    char *arguments[number_of_arguments];
    isolate_arguments_in(words, number_of_words, arguments);

    if(find_absolute_path(words[1], absolue_path) == false){
        printf("Could not find %s\n", words[1]);
        return 1;
    }

    int fd_in;
    int fd_out;

    if(find_file_directories(&fd_in, 
                            &fd_out, 
                            words, 
                            number_of_words) == false){

        return 1;

    }


    int pid = fork();
    if (pid == 0) {
        
        dup2(fd_in, STDIN_FILENO); 
        dup2(fd_out, STDOUT_FILENO);

        execve(absolue_path, arguments, NULL);
        printf("execve failed\n");
       
    }
    else if(pid != 0){

        int status;
        pid_t child_pid = waitpid(pid, &status, 0);

        if (child_pid == -1) {
            perror("waitpid");
            exit(1);
        }

        wait(NULL);
        close(fd_in);
        close(fd_out);

        printf("%s pid is %d. forked %d. "
           "Parent exiting\n",
            argv[0], getpid(), child_pid);

    }
    else {
        perror("Fork failed");
        exit(1);
    }
    
    return 0;
}


