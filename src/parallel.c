#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "bubblesort.h"

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

int main(int argc, char *argv[]) {
	int my_rank;
	int proc_n;
	MPI_Status status;
	int i;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

	if (my_rank == 0) {
		int array[ARRAY_SIZE];
		for (i = 0; i < ARRAY_SIZE; i++)
			array[i] = ARRAY_SIZE - i;

		printf("Root process unordered:\n");
		for (i = 0; i < ARRAY_SIZE; i++)
			printf("[%03d] ", array[i]);
		printf("\n");

		MPI_Send(array, ARRAY_SIZE / 2,
				 MPI_INT, 1, 1,
				 MPI_COMM_WORLD);

		MPI_Send(array + ARRAY_SIZE / 2, ARRAY_SIZE / 2,
				 MPI_INT, 2, 1,
				 MPI_COMM_WORLD);

		MPI_Recv(array, ARRAY_SIZE / 2,
				 MPI_INT, 1, MPI_ANY_TAG,
				 MPI_COMM_WORLD, &status);

		MPI_Recv(array + ARRAY_SIZE / 2, ARRAY_SIZE / 2,
				 MPI_INT, 2, MPI_ANY_TAG,
				 MPI_COMM_WORLD, &status);

		int *result = interleaving(array, ARRAY_SIZE);

		printf("Root process ordered:\n");
		for (i = 0; i < ARRAY_SIZE; i++)
			printf("[%03d] ", result[i]);
		printf("\n");


	} else {
		int array[ARRAY_SIZE / 2];
		MPI_Recv(array, ARRAY_SIZE / 2,
				 MPI_INT, 0, MPI_ANY_TAG,
				 MPI_COMM_WORLD, &status);

		bubble_sort(ARRAY_SIZE / 2, array);

		MPI_Send(array, ARRAY_SIZE / 2,
				 MPI_INT, 0, 1,
				 MPI_COMM_WORLD);

//		printf("Leaf process:\n");
//		for (i = 0; i < ARRAY_SIZE / 2; i++)
//			printf("[%03d] ", array[i]);
//		printf("\n");
	}

	MPI_Finalize();
	return 0;
}
