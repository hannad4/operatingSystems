/** Daniel Hanna
    CSC 345-01
    Lab 04 Exercise 1 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int n = atoi(argv[1]); 
    int i, j; 
    int count  = 0; 
    time_t begin = time(NULL); 
    pid_t id = getpid(); 

    for(i = 1; i <= n; ++i) {
        for(j = 2; j < i; ++j) {
            if (i % j == 0) {
                break; 
            }
        }
        if (j == i) {
            ++count; 
        }
    }
    printf("\n"); 
    printf("* Process %d found %d primes within [1, %d] in %ld seconds\n", id, count, n, time(NULL)-begin); 
    return 0; 

}