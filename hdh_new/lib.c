//
// Created by tai on 26/09/2019.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "lib.h"

void readInput(char* cmd, char** args, int* flag, char** getLast, int* run, char** fileName, char** cmd2){
    fflush(stdin);
    int length;
    char* temp = malloc(sizeof(char) * MAX_LINE);
    if (fgets(temp, MAX_LINE, stdin) != NULL)
    {
        length = strlen(temp);
        if (length == MAX_LINE-1 && temp[length-1]!='\n')
        {
            printf("Error, line overeached buffer!\n");
            *flag = 0;
        }
        if(strcmp(temp,"\n") == 0){
            *run = 0;
            return;
        }
        if(temp[length - 2] == '&'){
            *getLast = "&";
            temp[length - 2] = '\0';
        }
        else if(strcmp(temp,"!!") == 0){
            *getLast = "!!";
            if(args[0] == NULL)
            {
                *run = 0;
                printf("No commands in history.\n");
            }
            return;
        }
        if (temp[length-1]=='\n')
        {
            temp[length-1]='\0';
        }
        strcpy(cmd,temp);
        int i = 0;
        int j = 0;
        char*p = strtok(cmd," ");
        while(p!=NULL){
            if(strcmp(p,">") == 0)
            {
                *run = 2;
                break;
            }
            if(strcmp(p,"<") == 0)
            {
                *run = 3;
                break;
            }
            if(strcmp(p,"|") == 0){
                *run = 4;
                break;
            }
            args[i++] = p;
            p = strtok(NULL," ");
        }
        if(*run > 1){
            p = strtok(NULL," ");
            while(p != NULL) {
                if (*run < 4)
                    *fileName = p;
                else
                    cmd2[j++] = p;
                p = strtok(NULL," ");
            }
        }
        free(temp);
    }
}


