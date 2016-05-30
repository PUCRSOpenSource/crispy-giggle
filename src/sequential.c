#include <stdio.h>
#include <stdlib.h>
#include "bubblesort.h"

//#define DEBUG 1
#define ARRAY_SIZE 100000


int main(void)
{
	int vetor[ARRAY_SIZE];
	int i;

	for (i=0 ; i<ARRAY_SIZE; i++)
		vetor[i] = ARRAY_SIZE-i;


#ifdef DEBUG
	printf("\nVetor: ");
	for (i=0 ; i<ARRAY_SIZE; i++)
		printf("[%03d] ", vetor[i]);
#endif

	bubble_sort(ARRAY_SIZE, vetor);


#ifdef DEBUG
	printf("\nVetor: ");
	for (i=0 ; i<ARRAY_SIZE; i++)
		printf("[%03d] ", vetor[i]);
#endif

	return 0;
}
