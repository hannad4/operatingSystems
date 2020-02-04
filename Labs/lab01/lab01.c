#include <stdio.h>

int helloWorld() {
    int j = 0; 
    while (j < 4) {
        printf("HELLO\n"); 
        j++; 
    }
    return 0; 
}

int main(int argc, char** argv) {
    helloWorld(); 
}