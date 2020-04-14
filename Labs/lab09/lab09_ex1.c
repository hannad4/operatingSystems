/** Daniel Hanna
CSC 345-01
Lab 09 Exercise 1 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int total_frames;
int* FRAME;  

int total_refs; 
int* page_refs; 

int pf_fifo, pf_lru, pf_opt; 

void pr_fifo() {
    pf_fifo = 0; 
    int front = 0;
    int rear = total_frames-1;
    int size = 0;

    // implement FIFO to count pf_fifo

    // loop through page_refs
    for( int i = 0; i < total_refs; ++i) {
        int cur_ref = page_refs[i]; 
        int present = 0; 
        // for each page requested in page_refs
        // see if you already have that page in FRAME
        for(int j = 0; j < total_frames; ++j) {
            if(FRAME[j] == cur_ref) {
                present = 1; 
            }
        }
        // if so, then continue on the next page request
        if(present) {
            continue; 
        }
        // if not, then it IS a page fault (update pf_fifo)
        else {
            pf_fifo++; 

            // see if you have empty space in FRAME
                    // if so, add that page to FRAME
                    // if not, then find the oldest page in FRAME  <-- queue like implementation 
                        // replace the oldest page with the requested page
            if(size == total_frames) {
                front = (front+1) % total_frames; 
                size--; 
            }
            rear = (rear+1) % total_frames; 
        }
    }
}

void pr_lru() {
    pf_lru = 0; 
    int accesses[total_frames]; 

    // implement LRU  to count pf_lru
    // must consider and decide which page has not been used for the longest amount of time
    // implement this in stack as discussed in slide

    for(int i=0; i < total_frames; ++i) {
        accesses[i] = i; 
    }

    for(int j=0; j< total_refs; ++j) {
        int present = 0; 
        int found = 0; 

        for(int i = 0; i< total_frames; i++){
            if(page_refs[j] == FRAME[i]){
                found = 1;
                for(int h = 0; h < total_frames; h++){
                    accesses[h]--;

                }
                accesses[i] = total_frames - 1;
            }
        }

        if(found == 0){
            int replacement_frame = 0;
            int least = 0;

            for(int i = 0; i< total_frames; i++){
                if(replacement_frame >= accesses[i])
                    least = i;
            }

            replacement_frame = least;
            FRAME[replacement_frame] = page_refs[j];

            for(int h = 0; h< total_frames; h++){
                accesses[h]--;

            }
            accesses[replacement_frame] = total_frames - 1;
            pf_lru++;
        }
    }

}

void pr_opt() {
    pf_opt = 0;
    int futureDistances[total_frames];

    // implement optimal to count pf_opt 
    // must consider and decide which page will be referenced farthest away into the future

    for(int j = 0; j<total_refs;j++){

        for(int i = 0; i< total_frames; i++) {
            futureDistances[i] = 0;
        }

        int found = 0;

        for(int i = 0; i<total_frames;i++){
            if(page_refs[j] == FRAME[i])
                found = 1;
        } 

        if(found == 0){

            for(int i = 0; i< total_frames; i++){

                for(int h = j; h<total_refs; h++){
                    futureDistances[i]++;

                    if(FRAME[i] == page_refs[h]){
                        break;
                    }
                    else if(h == total_refs - 1){
                        futureDistances[i]++;
                    }
                }
            }

            int replacement_idx = 0;
            int max = 0;
            for(int i = 0; i< total_frames; i++){
                if(max < futureDistances[i]){
                    max = futureDistances[i];
                    replacement_idx = i;
                }

                else if(max == futureDistances[i]){
                    if(FRAME[i-1] != -1){
                        replacement_idx = i;
                    }
                }
            }

            FRAME[replacement_idx] = page_refs[j];
            pf_opt++;
        }
    }
}

int main(int argc, char** argv) {

    total_frames = atoi(argv[1]);
    if (total_frames < 1 || total_frames > 7) {
        printf("Total frames must be [1, 7] range\n"); 
        return -1; 
    } 

    FRAME = (int*)malloc(sizeof(int) * total_frames); 

    FILE* fp = fopen("input.txt", "rt"); 
    if (fp == NULL) return -1; 

    fscanf(fp, "%d", &total_refs); 

    page_refs = (int*)malloc(sizeof(int) * total_refs); 

    for (int i = 0; i < total_refs; ++i) {
        fscanf(fp, "%d", &(page_refs[i])); 
    }

    fclose(fp); 
    
    // check the input
    /*
    printf("total: %d\n", total_refs); 
    for (int i = 0; i < total_refs; ++i) {
        printf("%d ", page_refs[i]); 
    }
    printf("\n"); 
    */



   // Run FIFO
    pr_fifo(); 

   // Run LRU
    pr_lru(); 

   // Run optimal
    pr_opt(); 


    // report the result
    fp = fopen("output.txt", "wt"); 

    fprintf(fp, "FIFO: %d\n", pf_fifo); 
    fprintf(fp, "LRU: %d\n", pf_lru); 
    fprintf(fp, "Optimal: %d\n", pf_opt); 

    fclose(fp); 

    free(page_refs); 
    free(FRAME); 

    return 0; 
}