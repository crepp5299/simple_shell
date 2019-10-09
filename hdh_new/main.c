#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "lib.h"
#include<sys/stat.h>


int main()
{
    char* cmd = malloc(sizeof(char) * MAX_LINE); /* command line*/
    char** args = (char**)malloc(MAX_LINE*sizeof(char)); /* command line arguments*/
    char** cmd2 = (char**)malloc(MAX_LINE*sizeof(char));
    int isRunning = 1;
    while (isRunning) {
        int run = 1;
        printf("osh>");
        fflush(stdout);
        char* fileName;
        char* getLast;

        readInput(cmd, args, &isRunning, &getLast, &run, &fileName, cmd2);
        if(run != 0) {

            if (strcmp(args[0], "exit") == 0)
                break;
            pid_t pid;

            if ((pid = fork()) < 0) {
                printf("*ERROR:forking child process failed\n");
            } else if (pid == 0) {
                if(run == 4) {
                    int pipefd[2];
                    if (pipe(pipefd) == -1) {
                        fprintf(stderr, "Pipe Failed\n");
                        return 1;
                    }
                    int pid_pipe = fork();
                    if (pid_pipe > 0) {
                        wait(NULL);
                        close(pipefd[1]);
                        dup2(pipefd[0], STDIN_FILENO);
                        close(pipefd[0]);
                        if (execvp(cmd2[0], cmd2) == -1) {
                            printf("Invalid Command!\n");
                            return 1;
                        }
                    }
                    else if (pid_pipe == 0) {
                        close(pipefd[0]);
                        dup2(pipefd[1], STDOUT_FILENO);
                        close(pipefd[1]);
                        if (execvp(args[0], args) == -1) {
                            printf("Invalid Command!\n");
                            return 1;
                        }
                    }
                    close(pipefd[0]);
                    close(pipefd[1]);
                    break;
                }
                else {
                    int f;
                    int exe;
                    if (run == 2) {
                        f = open(fileName, O_WRONLY | O_CREAT, 0644);
                        dup2(f, STDOUT_FILENO);
                        exe = execvp(args[0], args);
                        close(STDOUT_FILENO);
                        close(f);
                    } else if (run == 3) {
                        f = open(fileName, O_RDONLY, 0644);
                        dup2(f, STDIN_FILENO);
                        exe = execvp(args[0], args);
                        close(STDIN_FILENO);
                        close(f);
                    } else {
                        exe = execvp(args[0], args);
                    }
                    if (exe < 0) {
                        printf("*ERROR:exec failed\n");
                    }
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
