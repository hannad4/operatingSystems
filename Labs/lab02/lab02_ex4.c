/** Daniel Hanna
CSC345-01
Lab 2 Exercise 4 */

#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <stdlib.h>     // Including stdlib to avoid compilation warning

int main(int argc, char** argv) {
    const char* name = "COLLATZ"; 
    const int SIZE = 4096; 
    int n = atoi(argv[1]); 
    int shm_fd; 
    void* ptr; 
    
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE); 
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); 
    pid_t id = fork(); 

    if (id == 0) {
        shm_fd = shm_open(name, O_RDWR, 0666);
        ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
       
        while(n>1) {
            sprintf(ptr, "%d ", n); 
            ptr+= sprintf(ptr, "%d ", n);
            if (n%2 == 0) {
                n = n/2; 
            }
            else {
                n = 3 * n + 1; 
            }
        } 
        sprintf(ptr, "%d ", n); 
        ptr+= sprintf(ptr, "%d ", n);
         
    }
    else {
        wait(NULL); 
        printf("PARENT: %s\n", (char*) ptr); 
        shm_unlink(name); 
    }
    return 0; 
}