#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "bubblesort.h"

#define ARRAY_SIZE 40
#define SIZE_TAG 0
#define ARRAY_TAG 1

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

int left_child(int index) {
	return 2 * index + 1;
}

int right_child(int index) {
	return 2 * index + 2;
}

int parent(int index) {
	return (index - 1) / 2;
}

int is_leaf(int current_size, int total_size, int number_of_process) {
	return current_size <= (total_size / (number_of_process - 1));
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
		int half_size = ARRAY_SIZE / 2;

		for (i = 0; i < ARRAY_SIZE; i++)
			array[i] = ARRAY_SIZE - i;

		printf("Root process unordered:\n");
		for (i = 0; i < ARRAY_SIZE; i++)
			printf("[%03d] ", array[i]);
		printf("\n");

		// Send size
		MPI_Send(&half_size, 1,
				 MPI_INT, right_child(my_rank), SIZE_TAG,
				 MPI_COMM_WORLD);

		MPI_Send(&half_size, 1,
				 MPI_INT, left_child(my_rank), SIZE_TAG,
				 MPI_COMM_WORLD);

		// Send array
		MPI_Send(array, ARRAY_SIZE / 2,
				 MPI_INT, right_child(my_rank), ARRAY_TAG,
				 MPI_COMM_WORLD);

		MPI_Send(array + ARRAY_SIZE / 2, ARRAY_SIZE / 2,
				 MPI_INT, left_child(my_rank), ARRAY_TAG,
				 MPI_COMM_WORLD);

		// Receive response
		MPI_Recv(array, ARRAY_SIZE / 2,
				 MPI_INT, 1, ARRAY_TAG,
				 MPI_COMM_WORLD, &status);

		MPI_Recv(array + ARRAY_SIZE / 2, ARRAY_SIZE / 2,
				 MPI_INT, 2, ARRAY_TAG,
				 MPI_COMM_WORLD, &status);

		int *result = interleaving(array, ARRAY_SIZE);

		printf("Root process ordered:\n");
		for (i = 0; i < ARRAY_SIZE; i++)
			printf("[%03d] ", result[i]);
		printf("\n");


	} else {
		int size;

		MPI_Recv(&size, 1,
				 MPI_INT, parent(my_rank), SIZE_TAG,
				 MPI_COMM_WORLD, &status);

		int *array = calloc(size, sizeof(int));

		MPI_Recv(array, size,
				 MPI_INT, parent(my_rank), ARRAY_TAG,
				 MPI_COMM_WORLD, &status);

		if(is_leaf(size, ARRAY_SIZE, proc_n)) {
			bubble_sort(size, array);

			MPI_Send(array, size,
					 MPI_INT, parent(my_rank), ARRAY_TAG,
					 MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
}

