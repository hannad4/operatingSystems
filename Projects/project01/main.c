/** Daniel Hanna
CSC345-01
Project 1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#define MAX_LINE 80
#define MAX_HISTORY 1

void runCommand(char **argv) {      /* run the user's input */
    pid_t pid;
    pid = fork();  
    if(pid == 0) {
        if(execvp(*argv, argv) < 0) {       /* make sure the input is a runnable command */
            printf("Error: Command not found\n"); 
            exit(1); 
        }
    }
    else {
        wait(NULL); 
    }
}

void parseInput(char *input, char **argv) {         /* convert the user's ending character to \0 for execvp */
    while(*input != '\0') {
        while(*input == ' ' || *input == '\t' || *input == '\n') {
            *input++ = '\0'; 
        }
        *argv++ = input; 
        while(*input != '\0' && *input != ' ' && *input != '\t' && *input != '\n') {
            input++; 
        }
    }
    *argv = '\0'; 
}

void runHistory(char *hist[], int currentInput, char *input, char **argv) {     /* check if the user asked for history and run it if so */
    if(strcmp(++input, "!") == 0) {
        if(hist[(currentInput)]) {
            printf("%s\n", hist[(currentInput)]);
            parseInput(hist[(currentInput)], argv); 
            runCommand(argv); 
        }
        else {
            printf("Error: No history found\n"); 
        }
        
    }
}

int main(void) {
    int should_run = 1; 
    char input[MAX_LINE]; 
    char *argv[MAX_LINE];
    char *hist[MAX_HISTORY]; 

    int currentInput = 0; 
    for(int i = 0; i < 1; i++) {        /* history should initally be null */
        hist[i] = NULL; 
    }

    while(should_run) {
        printf("osh>"); 
        fflush(stdout); 
        fgets(input, MAX_LINE, stdin);  /* get the users input */

        if(input[strlen(input) - 1] == '\n') {      /* check the ending character, convert if needed */
            input[strlen(input) - 1] = '\0';
        }

        if(strncmp(input, "!!", 1) != 0 && strncmp(input, "\033", 1) != 0) {    /* keep track of history */
            free(hist[currentInput]); 
            hist[currentInput] = strdup(input); 
            currentInput = (currentInput + 1) % 1; 
        }

        if(strncmp(input, "!!", 1) == 0) {          /* if user requests history, then run the previous input */
            runHistory(hist, currentInput, input, argv); 
        }

        else if(strstr(input, " > ") || strstr(input, " < ") || strstr(input, " | ") || strstr(input, " &")) { /* check redirect, pipe, and & symbols */
            int i = 0;
            int j = 0; 
            int k = 0; 
            parseInput(input, argv);
            for (i = 0; i <= sizeof(argv) - 1; i++){        /* use variable i to keep track of "<" or ">" location */
                if (strncmp(argv[i], "<", MAX_LINE) == 0 || strncmp(argv[i], ">", MAX_LINE) == 0){
                    break;
                }
                else if (strncmp(argv[i], "&", MAX_LINE) == 0) { /* use variable j to keep track of "&" location */
                    j = i; 
                    break; 
                }
                else if (strncmp(argv[i], "|", MAX_LINE) == 0) { /* use variable k to keep track of "|" location */
                    k = i; 
                    break; 
                }
            }
            
            if(strcmp(argv[i], ">") == 0) {   /* if > exists, use dup2 for redirection */
                int stdOutCopy = dup(STDOUT_FILENO);   
                int file = open(argv[i+1], O_RDWR | O_CREAT, 0644); 
                argv[i] = NULL;     /* set the ">" and other argument null so execvp can run with no complaints */
                argv[i+1] = NULL; 
                dup2(file, STDOUT_FILENO); 
                close(file); 
                pid_t pid; 
                pid = fork(); 
                if(pid == 0) {
                    if(execvp(argv[0], argv) < 0) {
                        printf("Error: Command not found\n"); 
                        exit(1); 
                    }
                }
                else {
                    wait(NULL); 
                }
                fflush(stdout); 
                dup2(stdOutCopy, STDOUT_FILENO); 
                close(stdOutCopy); 
            }  

            else if(strcmp(argv[i], "<") == 0) { /* use dup2 for < redirection, but this time using STDIN_FILENO */
                int stdInCopy = dup(STDIN_FILENO);
                int file = open(argv[i+1], O_RDWR | O_CREAT, 0644); 
                argv[i] = NULL;     /* set the ">" and other argument null so execvp can run with no complaints */
                argv[i+1] = NULL; 
                dup2(file, STDIN_FILENO); 
                close(file); 
                pid_t pid;
                pid = fork();  
                if(pid == 0) {
                if(execvp(argv[0], argv) < 0) {
                    printf("Error: Command not found\n"); 
                    exit(1); 
                    }
                }
                else {
                    wait(NULL); 
                } 
                fflush(stdin); 
                dup2(stdInCopy, STDIN_FILENO); 
                close(stdInCopy);  
            }

            else if(strcmp(argv[j], "&") == 0) {    /* if "&", then avoid doing wait(NULL). Use signal() to avoid zombie child issue */
                argv[j] = NULL; 
                pid_t pid; 
                pid = fork(); 
                if(pid == 0) {
                    if(execvp(argv[0], argv) < 0) {
                        printf("Error: Command not found\nosh>"); 
                        exit(1); 
                    }
                }
                else {
                    signal(SIGCHLD,SIG_IGN); 
                }
            }

            else if(strcmp(argv[k], "|") == 0) {    /* partially implemented: opening pipe via file descriptors */
                char *secCommand[MAX_LINE]; 
                secCommand[0] = strdup(argv[k+1]);
                argv[k] = NULL; 
                argv[k+1] = NULL; 
                int fd1[2];
                pid_t pid; 
                pid = fork();  
                if(pid == 0) { 
                    pid_t pid2; 
                    pid2 = fork(); 
                    if(pid2 == 0) {
                        close(fd1[1]); 
                        dup2(fd1[0], 0);
                        close(fd1[0]);
                        execvp(secCommand[0], secCommand);  
                    }
                    else {
                        wait(NULL); 
                        close(fd1[0]);
                        dup2(fd1[1], 1); 
                        close(fd1[1]);
                        execvp(argv[0], argv);                        
                    }
                }
                else {
                    wait(NULL); 
                } 
            }

            else{
                printf("Error: Command not found\n"); 
            }
        }

        else if(strcmp(input,"quit") == 0 || strcmp(input, "exit") == 0) {  /* if quit or exit (because i forget to only use quit), exit the program */
            should_run = 0; 
            break; 
        }

        else {
            parseInput(input, argv); 
            if(strcmp(input, "cd") == 0) {  /* if user wants to cd, check if directory exists, and change directory to it */
                DIR* dir = opendir(argv[1]); 
                if(dir) {
                    chdir(argv[1]);
                }
                else {
                    printf("Error: No such file or directory\n");   /* otherwise, the directory doesn't exist */
                }   
            }

            else {  /* execute the user's input */
                runCommand(argv); 
            }
        }
    }
    return 0; 
}