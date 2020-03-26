/** Daniel Hanna
CSC345-01
Lab 8 Exercise 1 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	unsigned long addr = atoi(argv[1]);
	unsigned long page =  addr >> 12;
	unsigned long offset = addr & 0xfff;

	printf("The address %lu contains: \n", addr);
	printf("page number = %lu\n", page);
	printf("offset = %lu\n", offset);
}