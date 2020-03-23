/**	Daniel Hanna		
CSC345-01
Lab	6 Exercise	1 */

/* import needed stuff */
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>

/* define max time for waiting */
#define MAX_WAIT 3

/* define farmer  */
typedef struct _FARMER
{
	pthread_t tid; 
    int isNorth; 
	int idx; 
    int waitfor; 
} FARMER;


/* bridge */
sem_t bridge; 

void enter_bridge(FARMER* f) {

    if(f->isNorth == 1) {   /* notify who is lined up on the bridge */
        printf("North farmer %d lined up the bridge.\n", f->idx); 
    }
    else {
        printf("South farmer %d lined up the bridge.\n", f->idx); 
    }

    sem_wait(&bridge);  /* lock bridge for usage */
    if(f->isNorth == 1) {
        printf("North farmer %d just entered the bridge.\n", f->idx); 
    }
    else {
        printf("South farmer %d just entered the bridge.\n", f->idx); 
    }
} 

void exit_bridge(FARMER* f) {

    if(f->isNorth == 1) {   /* notify who's leaving bridge */
        printf("North farmer %d about to leave the bridge.\n", f->idx); 
    }
    else {
        printf("South farmer %d about to leave the bridge.\n", f->idx); 
    }

    sem_post(&bridge);      /* unlock the bridge */

    if(f->isNorth == 1) {
        printf("North farmer %d just left the bridge.\n", f->idx); 
    }
    else {
        printf("South farmer %d just left the bridge.\n", f->idx); 
    }

} 

void* pass_bridge(void* param) {
    FARMER* f = (FARMER*)param; 

    /* entry section */
    enter_bridge(f);        /* have the farmer etner the bridge */

    /* critical section */
    if(f->isNorth == 1) {
        printf("  North farmer %d is passing bridge in %d seconds.\n", f->idx, f->waitfor); 
    }
    else {
        printf("  South farmer %d is passing bridge in %d seconds.\n", f->idx, f->waitfor);
    }
    sleep(f->waitfor);  /* need to wait for however long it will take farmer to cross */
    
    /* exit section */
    exit_bridge(f);     /* have the farmer exit the bridge */
    pthread_exit(0); 
}

int main(int argc, char** argv) {
    sem_init(&bridge, 0, 1);        /* initialize the semaphore */
    int nNorth = atoi(argv[1]);     /* get the number of farmers */
    int nSouth = atoi(argv[2]); 

    /* create arrays of farmers */
    FARMER* fNorth = (FARMER*)malloc(sizeof(FARMER) * nNorth); 
    FARMER* fSouth = (FARMER*)malloc(sizeof(FARMER) * nSouth); 

    /* prepare farmer information */
    for(int i = 0; i<nNorth; ++i) {
        fNorth[i].isNorth = 1; 
        fNorth[i].idx = i; 
        fNorth[i].waitfor = rand() % MAX_WAIT; 
    }
    for(int i = 0; i<nSouth; ++i) {
        fSouth[i].isNorth = 0; 
        fSouth[i].idx = i; 
        fSouth[i].waitfor = rand() % MAX_WAIT; 
    }

    /* create threads */
    for(int i = 0; i< nNorth; ++i) {
        pthread_create(&(fNorth[i].tid), NULL, pass_bridge, &(fNorth[i])); 
    }
    for(int i =0; i< nSouth; ++i) {
        pthread_create(&(fSouth[i].tid), NULL, pass_bridge, &(fSouth[i])); 
    }

    /* wait for all farmers to pass the bridge */
    for(int i = 0; i< nNorth; ++i) {
        pthread_join(fNorth[i].tid, NULL); 
    }
    for(int i =0; i< nSouth; ++i) {
        pthread_join(fSouth[i].tid, NULL); 
    }

    /* delete farmer arrays */
    free(fNorth); 
    free(fSouth); 

    sem_destroy(&bridge); 
    return 0; 
}