/** Daniel Hanna
 CSC 345-01
 Lab 5 Exercise 3 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int total_pts;
int pts_in_circle; 
pthread_mutex_t lock;

void *runner(void *param) {
    int points = *((int *)param); 
    for (int i = 0; i < points; ++i) {
        double x = (double)rand() / (double)RAND_MAX; 
        double y = (double)rand() / (double)RAND_MAX; 

        x = x * 2.0 - 1.0; 
        y = y * 2.0 - 1.0; 

        if(sqrt(x*x+y*y) < 1.0) {
            pthread_mutex_lock(&lock);
            ++pts_in_circle; 
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(0); 
}

int main(int argc, char** argv) {
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    total_pts = atoi(argv[1]); 

    pthread_mutex_init(&lock, NULL);
    int point_set1 = (total_pts / 4); 
    int point_set2 = (total_pts / 4);
    int point_set3 = (total_pts / 4);
    int point_set4 = (total_pts - (point_set1 + point_set2 + point_set3)); 
    

    pthread_create(&tid1, NULL, runner, &point_set1); 
    pthread_create(&tid2, NULL, runner, &point_set2); 
    pthread_create(&tid3, NULL, runner, &point_set3); 
    pthread_create(&tid4, NULL, runner, &point_set4); 
    
    pthread_join(tid1, NULL); 
    pthread_join(tid2, NULL); 
    pthread_join(tid3, NULL); 
    pthread_join(tid4, NULL); 

    double pi_e = 4.0 * (double)pts_in_circle / (double)total_pts;

    printf("PI estimate = %f\n", pi_e); 

    pthread_mutex_destroy(&lock);
    return 0; 
}