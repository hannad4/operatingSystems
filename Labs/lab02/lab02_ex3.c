/** Daniel Hanna
CSC345-01
Lab 2 Exercise 3 */

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char** argv){
    pid_t pid;

    pid = fork();

    if(pid == 0){
        //zombie
    }
    else if(pid > 0){
        //parent process
        //not invoke wait 
        //while(1);
        sleep(30);
    }
    return 0;
}