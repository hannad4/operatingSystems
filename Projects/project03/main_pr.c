/** Daniel Hanna | Garrett Hope
CSC 345-01
Project 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define NUM_FRAMES 128
#define NUM_PAGES 128
#define FRAME_SIZE 256
#define PAGE_SIZE 256
#define TLB_SIZE 16

char address[BUFFER_SIZE];
signed char physical_mem[NUM_FRAMES][FRAME_SIZE];

int frame_counter = 0;
int TLB_counter = 0;
int TLB_hits = 0;
int page_faults = 0;
int num_addresses;


FILE *backing_store;
FILE *address_file;
FILE *fp1;
FILE *fp2;
FILE *fp3;

typedef struct
{
    int page_number;
    int frame_number;
} pfn;

pfn TLB[TLB_SIZE];
pfn page_table[NUM_PAGES];


void readBackingStore(int page_num, int frame_num)
{
    if (fseek(backing_store, page_num * PAGE_SIZE, SEEK_SET) != 0)
    {
        fprintf(stderr, "Error seeking BACKING_STORE.bin\n");
    }

    if (fread(physical_mem[frame_num], sizeof(signed char), PAGE_SIZE, backing_store) == 0)
    {
        fprintf(stderr, "Error reading BACKING_STORE.bin\n");
    }
}

int checkTLB(int page_num)
{
    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (TLB[i].page_number == page_num)
            return TLB[i].frame_number;
    }
    return -1;
}

void populateTLB(int page_num, int frame_num)
{
    TLB[TLB_counter].page_number = page_num;
    TLB[TLB_counter].frame_number = frame_num;

    TLB_counter = (TLB_counter + 1) % TLB_SIZE;
}

int checkPageTable(int page_num){
    for (int i = 0; i < NUM_PAGES; i++){
        if (page_table[i].page_number == page_num){
            return page_table[i].frame_number;
        }
    }
    return -1;
}

void translateAddress(int address)
{
    int frame_num;
    int page_num = ((address & 0xFFFF) >> 8);
    int offset = (address & 0xFF);

    frame_num = checkTLB(page_num);

    if (frame_num == -1)
    {
        frame_num = checkPageTable(page_num);

        if (frame_num == -1)
        {
            frame_num = frame_counter++ % NUM_PAGES;
            page_table[frame_num].page_number = page_num;
            page_table[frame_num].frame_number = frame_num;

            readBackingStore(page_num, frame_num);
            page_faults++;
        }
        populateTLB(page_num, frame_num);
    }
    else
    {
        TLB_hits++;
    }

    int physical_address = (frame_num << 8) | offset;
    int value = physical_mem[frame_num][offset];
    printf("Virtual Adress: %d Physical Address: %d, Value: %d\n", address, physical_address, value);
    fprintf(fp2, "%d\n", physical_address);
    fprintf(fp3, "%d\n", value);
}

int main(int argc, char **argv)
{
    address_file = fopen(argv[1], "r");
    backing_store = fopen("BACKING_STORE.bin", "rb");

    if (address_file == NULL)
    {
        printf("Address file could not be opened\n");
        exit(1);
    }

    fp1 = fopen("out1.txt", "wt");
    fp2 = fopen("out2.txt", "wt");
    fp3 = fopen("out3.txt", "wt");

    for (int i = 0; i < NUM_PAGES; i++)
    {
        page_table[i].page_number = -1;
        page_table[i].frame_number = -1;
    }

    for (int i = 0; i < TLB_SIZE; i++)
    {
        TLB[i].page_number = -1;
        TLB[i].frame_number = -1;
    }

    while (fgets(address, BUFFER_SIZE, address_file) != NULL)
    {
        num_addresses++;
        int logical_address = atoi(address);
        fprintf(fp1, "%d\n", logical_address);
        translateAddress(logical_address);
    }
    printf("PAGE FAULTS: %d\n", page_faults);
    printf("Page-fault rate: %f\n", (float)(page_faults)/ (float)(num_addresses));
    printf("TLB HITS: %d\n", TLB_hits);
    printf("TLB hit rate: %f\n", (float)(TLB_hits)/(float)(num_addresses));
    fclose(backing_store);
    fclose(address_file);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}