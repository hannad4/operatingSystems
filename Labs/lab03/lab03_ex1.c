/**	Daniel Hanna		
CSC345-01	
Lab 3 Exercise 1 **/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int len = 7;
int* nums; 
int r_min;
int r_max; 
float r_med;
float r_avg; 
float r_stddev; 

void *myMin (void *param) {
    int i;
    r_min =	nums[0];
    for ( i=0; i<len; ++i )	{
        if (r_min > nums[i])	
            r_min =	nums[i];
    }
    pthread_exit(0);
}

void *myMax (void *param) {
    int i;
    r_max =	nums[0];
    for ( i=0; i<len; ++i )	{
        if (r_max < nums[i])	
            r_max =	nums[i];
    }
    pthread_exit(0);
}

void *myAvg (void *param) {
    int i;
    r_avg =	0.0f;
    for ( i=0; i<len; i++ )	{
        r_avg += nums[i]; 
    }
    r_avg /= len; 
    pthread_exit(0);
}

void *mySD(void *param) {
    int i;
    r_avg = 0.0f; 
    for (i = 0; i < len; i++) {
        r_avg += nums[i];
    }
    r_avg /= len;
    for (i = 0; i < len; ++i) {
        r_stddev += pow(nums[i] - r_avg, 2);
    }
    r_stddev /= len; 
    r_stddev = sqrt(r_stddev); 
    pthread_exit(0); 
}

int cmpfunc(const void *a, const void *b) {
    return(*(int*)a - *(int*)b); 
}

void *myMed (void *param)
{
    qsort(nums, len, sizeof(int), cmpfunc);  

	if(len%2 == 0) {
		r_med = (nums[(len-1)/2] + nums[len/2])/2.0;
        pthread_exit(0);
    } 
	else {
		r_med = nums[len/2];
        pthread_exit(0); 
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("error\n"); 
        return 1; 
    }
    len = argc - 1; 
    nums = (int*)malloc(sizeof(int)*len); 
    for (int i=1; i<argc; ++i) {
        nums[i-1]=atoi(argv[i]); 
    }
    len = argc -1; 
    
    pthread_t tid;
    pthread_t tid1; 
    pthread_t tid2; 
    pthread_t tid3; 
    pthread_t tid4; 
    pthread_create(&tid, 0, myMin, NULL);
    pthread_create(&tid1, 0, myAvg, NULL);
    pthread_create(&tid2, 0, myMax, NULL); 
    pthread_create(&tid3, 0, myMed, NULL); 
    pthread_create(&tid4, 0, mySD, NULL);  
    pthread_join(tid,NULL);
    pthread_join(tid1, NULL); 
    pthread_join(tid2, NULL); 
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    printf("Min=%d\n",r_min);
    printf("Max=%d\n", r_max);  
    printf("Avg=%f\n", r_avg); 
    printf("Med=%f\n", r_med);
    printf("SDev=%f\n", r_stddev); 
    free(nums); 
    return	0;
}