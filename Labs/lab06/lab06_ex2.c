/**	Daniel Hanna		
CSC345-01
Lab	6 Exercise	2 */

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>

#define MAX_WAIT 3

typedef struct _FARMER
{
	pthread_t tid; 
    int isNorth; 
	int idx; 
    int waitfor; 
} FARMER;

pthread_mutex_t c_mutex; 
pthread_cond_t c_condv; 

int numWaitNorth, numWaitSouth, onBridge, prev; 

void enter_bridge_north(){
	
	++numWaitNorth;
	pthread_mutex_lock(&c_mutex);
	while((onBridge == 1) || (prev == 0 && numWaitSouth > 0)){
        pthread_cond_wait(&c_condv, &c_mutex);
    }   

	pthread_mutex_unlock(&c_mutex);
	--numWaitNorth;
	onBridge = 1;
	prev = 0;
}

void exit_bridge_north(){
	onBridge = 0;
	pthread_mutex_lock(&c_mutex);
	pthread_cond_broadcast(&c_condv);
	pthread_mutex_unlock(&c_mutex);
}

void enter_bridge_south(){
	
	++numWaitSouth;
	pthread_mutex_lock(&c_mutex);
	while((onBridge == 1) || (prev == 1 && numWaitNorth > 0)){
		pthread_cond_wait(&c_condv, &c_mutex);
	}
	pthread_mutex_unlock(&c_mutex);
	--numWaitSouth;
	onBridge = 1;
	prev = 1;
}

void exit_bridge_south(){
	onBridge = 0;
	pthread_mutex_lock(&c_mutex);
    pthread_cond_broadcast(&c_condv);
    pthread_mutex_unlock(&c_mutex);
}
 
void* pass_bridge(void* param) {
    FARMER* f = (FARMER*)param;  

    /* entry section */
    if(f->isNorth == 1) {
        printf("North farmer %d is passing bridge in %d seconds.\n", f->idx, f->waitfor); 
        
        enter_bridge_north(f);
    }
    else {
        printf("South farmer %d is passing bridge in %d seconds.\n", f->idx, f->waitfor);
        enter_bridge_south(f); 
    }
    
    sleep(f->waitfor); 
    
    /* exit section */
    if(f->isNorth == 1) {
        exit_bridge_north(); 
        printf(" North farmer %d passed the bridge\n", f->idx); 
    }
    else {
        exit_bridge_south(); 
        printf(" South farmer %d passed the bridge\n", f->idx);
    } 
    pthread_exit(0); 
}


int main(int argc, char** argv) {
    int nNorth = atoi(argv[1]); 
    int nSouth = atoi(argv[2]); 

    /* create arrays of farmers */
    FARMER* fNorth = (FARMER*)malloc(sizeof(FARMER) * nNorth); 
    FARMER* fSouth = (FARMER*)malloc(sizeof(FARMER) * nSouth); 
    
    pthread_mutex_init(&c_mutex, NULL); 
    pthread_cond_init(&c_condv, NULL); 
    
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

    pthread_mutex_destroy(&c_mutex); 
    pthread_cond_destroy(&c_condv); 

    /* delete farmer arrays */
    free(fNorth); 
    free(fSouth); 

    return 0; 
}