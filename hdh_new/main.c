#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "lib.h"
#include<sys/stat.h>
#include <fcntl.h>

int main()
{
    char* cmd = malloc(sizeof(char) * MAX_LINE); /* command line*/
    char** args = (char**)malloc(MAX_LINE*sizeof(char)); /* command line arguments*/
    int isRunning = 1;
    while (isRunning) {
        int run = 1;
        printf("osh>");
        fflush(stdout);
        char* fileName;
        char* getLast;
        readInput(cmd, args, &isRunning, &getLast, &run, &fileName);
        if(run != 0) {

            if (strcmp(args[0], "exit") == 0)
                break;
            pid_t pid;

            if ((pid = fork()) < 0) {
                printf("*ERROR:forking child process failed\n");
            } else if (pid == 0) {
                int f;
                int exe;
                if(run == 2) {
                    f = open(fileName, O_WRONLY | O_CREAT);
                    dup2(f, STDOUT_FILENO);
                    exe = execvp(args[0], args);
                    close(STDOUT_FILENO);
                    close(f);
                }
                else if(run == 3){
                    f = open(fileName, O_RDONLY | O_CREAT);
                    dup2(f, STDIN_FILENO);
                    exe = execvp(args[0], args);
                    close(STDIN_FILENO);
                    close(f);
                }
                else{
                    exe = execvp(args[0], args);
                }
                if (exe < 0) {
                    printf("*ERROR:exec failed\n");
                }
            } else {
                if (strcmp(getLast, "&") != 0) {
                    wait(NULL);
                }
            }
        }
    }
    return 0;
}
