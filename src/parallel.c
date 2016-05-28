#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 40

int *interleaving(int array[], int size) {
	int *aux_array;
	int i1, i2, i_aux;

	aux_array = calloc(size, sizeof(int));

	i1 = 0;
	i2 = size / 2;

	for (i_aux = 0; i_aux < size; i_aux++) {
		if (((array[i1] <= array[i2]) && (i1 < (size / 2))) || (i2 == size))
			aux_array[i_aux] = array[i1++];
		else
			aux_array[i_aux] = array[i2++];
	}

	return aux_array;
}

int main() {
	int vetor[ARRAY_SIZE];
	int *vetor2;
	int i;

	for (i = 0; i < ARRAY_SIZE / 2; i++)
		vetor[i] = i;
	for (i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++)
		vetor[i] = i - ARRAY_SIZE / 2;

	printf("\nVetor: ");
	for (i = 0; i < ARRAY_SIZE; i++)
		printf("[%03d] ", vetor[i]);

	vetor2 = interleaving(vetor, ARRAY_SIZE);


	printf("\nVetor: ");
	for (i = 0; i < ARRAY_SIZE; i++)
		printf("[%03d] ", vetor2[i]);

	return 0;
}

