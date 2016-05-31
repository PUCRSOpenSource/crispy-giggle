## sequential.c
```c
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
```
## parallel.c

```c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "bubblesort.h"

#define ARRAY_SIZE 100000
//#define DEBUG 1
#define SIZE_TAG 0
#define SEND_UP_TAG 1
#define SEND_DOWN_TAG 2

int
main(int argc, char *argv[])
{
	double t1, t2;
	t1 = MPI_Wtime();

	int my_rank;
	int proc_n;
	MPI_Status status;
	int i;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

	if (my_rank == 0)
	{
		int array[ARRAY_SIZE];
		for (i = 0; i < ARRAY_SIZE; i++)
			array[i] = ARRAY_SIZE - i;

		#ifdef DEBUG
		printf("Root process unordered:\n");
		for (i = 0; i < ARRAY_SIZE; i++)
			printf("[%03d] ", array[i]);
		printf("\n");
		#endif

		int *result = order(my_rank, array, ARRAY_SIZE, status);

		#ifdef DEBUG
		printf("Root process ordered:\n");
		for (i = 0; i < ARRAY_SIZE; i++)
			printf("[%03d] ", result[i]);
		printf("\n");
		#endif
		
	}
	else
	{
		int size;

		MPI_Recv(&size, 1,
				 MPI_INT, parent(my_rank), SIZE_TAG,
				 MPI_COMM_WORLD, &status);

		int *array = calloc(size, sizeof(int));

		MPI_Recv(array, size,
				 MPI_INT, parent(my_rank), SEND_DOWN_TAG,
				 MPI_COMM_WORLD, &status);

		if(is_leaf(size, ARRAY_SIZE, proc_n))
		{
			bubble_sort(size, array);

			MPI_Send(array, size,
					 MPI_INT, parent(my_rank), SEND_UP_TAG,
					 MPI_COMM_WORLD);
		}
		else
		{
			array = order(my_rank, array, size, status);
			MPI_Send(array, size,
					 MPI_INT, parent(my_rank), SEND_UP_TAG,
					 MPI_COMM_WORLD);
		}
	}

	if (my_rank == 0) {
		t2 = MPI_Wtime();
		printf( "Elapsed time is %f\n", t2 - t1 );
	}
	MPI_Finalize();
	return 0;
}

int*
order(int my_rank, int array[], int size, MPI_Status status)
{
	int half_size = size / 2;
	// Send size
	MPI_Send(&half_size, 1,
			MPI_INT, left_child(my_rank), SIZE_TAG,
			MPI_COMM_WORLD);

	MPI_Send(&half_size, 1,
			MPI_INT, right_child(my_rank), SIZE_TAG,
			MPI_COMM_WORLD);

	// Send array
	MPI_Send(array, half_size,
			MPI_INT, left_child(my_rank), SEND_DOWN_TAG,
			MPI_COMM_WORLD);

	MPI_Send(array + half_size, half_size,
			MPI_INT, right_child(my_rank), SEND_DOWN_TAG,
			MPI_COMM_WORLD);

	// Receive response
	MPI_Recv(array, half_size,
			MPI_INT, left_child(my_rank), SEND_UP_TAG,
			MPI_COMM_WORLD, &status);

	MPI_Recv(array + half_size, half_size,
			MPI_INT, right_child(my_rank), SEND_UP_TAG,
			MPI_COMM_WORLD, &status);

	return interleaving(array, size);
}
```
## parallel_optimized.c

```c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "bubblesort.h"

#define ARRAY_SIZE 100000
//#define DEBUG 1
#define SIZE_TAG 0
#define SEND_UP_TAG 1
#define SEND_DOWN_TAG 2

int
main(int argc, char *argv[])
{
	double t1, t2;
	t1 = MPI_Wtime();
	int my_rank;
	int proc_n;
	MPI_Status status;
	int i;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n);
	int piece_size = ARRAY_SIZE / proc_n;

	if (my_rank == 0)
	{
		int array[ARRAY_SIZE];
		for (i = 0; i < ARRAY_SIZE; i++)
			array[i] = ARRAY_SIZE - i;

#ifdef DEBUG
		printf("Root process unordered:\n");
		for (i = 0; i < ARRAY_SIZE; i++)
			printf("[%03d] ", array[i]);
		printf("\n");
#endif

		int *result = order(my_rank, array, ARRAY_SIZE, status, piece_size);

#ifdef DEBUG
		printf("Root process ordered:\n");
		for (i = 0; i < ARRAY_SIZE; i++)
			printf("[%03d] ", result[i]);
		printf("\n");
#endif

	}
	else
	{
		int size;

		MPI_Recv(&size, 1,
				 MPI_INT, parent(my_rank), SIZE_TAG,
				 MPI_COMM_WORLD, &status);

		int *array = calloc(size, sizeof(int));

		MPI_Recv(array, size,
				 MPI_INT, parent(my_rank), SEND_DOWN_TAG,
				 MPI_COMM_WORLD, &status);

		if(is_leaf3(size, ARRAY_SIZE, proc_n))
		{
			bubble_sort(size, array);

			MPI_Send(array, size,
					 MPI_INT, parent(my_rank), SEND_UP_TAG,
					 MPI_COMM_WORLD);
		}
		else
		{
			array = order(my_rank, array, size, status, piece_size);
			MPI_Send(array, size,
					 MPI_INT, parent(my_rank), SEND_UP_TAG,
					 MPI_COMM_WORLD);
		}
	}
	if (my_rank == 0) {
		t2 = MPI_Wtime();
		printf( "Elapsed time is %f\n", t2 - t1 );
	}
	MPI_Finalize();

	return 0;
}

int*
order(int my_rank, int array[], int size, MPI_Status status, int piece_size)
{

	int third_size = (size - piece_size) / 2;
	int left_size = size - 2 * third_size;
	// Send size
	MPI_Send(&third_size, 1,
			 MPI_INT, left_child(my_rank), SIZE_TAG,
			 MPI_COMM_WORLD);

	MPI_Send(&third_size, 1,
			 MPI_INT, right_child(my_rank), SIZE_TAG,
			 MPI_COMM_WORLD);

	// Send array
	MPI_Send(array, third_size,
			 MPI_INT, left_child(my_rank), SEND_DOWN_TAG,
			 MPI_COMM_WORLD);

	MPI_Send(array + third_size, third_size,
			 MPI_INT, right_child(my_rank), SEND_DOWN_TAG,
			 MPI_COMM_WORLD);

	//order rest locally

	bubble_sort(left_size, array + 2 * third_size);

	// Receive response
	MPI_Recv(array, third_size,
			 MPI_INT, left_child(my_rank), SEND_UP_TAG,
			 MPI_COMM_WORLD, &status);

	MPI_Recv(array + third_size, third_size,
			 MPI_INT, right_child(my_rank), SEND_UP_TAG,
			 MPI_COMM_WORLD, &status);

	return interleaving3(array, size, piece_size);
}
```

## utils.c

```c
#include <stdio.h>
#include <stdlib.h>

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

int valid(int index, int limit);
int higher_or_invalid(int array[], int value_index, int limit, int other_value_index);
int *interleaving3(int array[], int size, int slice_size) {
	int *aux_array;
	int i1, i2, i3, i1_limit, i2_limit, i3_limit, i_aux;

	int third_size = (size - slice_size) / 2;

	aux_array = calloc(size, sizeof(int));

	i1 = 0;
	i1_limit = third_size;
	i2 = third_size;
	i2_limit = 2 * third_size;
	i3 = 2 * third_size;
	i3_limit = size;

	for (i_aux = 0; i_aux < size; i_aux++) {

		if (valid(i1, i1_limit) &&
				higher_or_invalid(array, i2, i2_limit, i1) &&
				higher_or_invalid(array, i3, i3_limit, i1)) {
			aux_array[i_aux] = array[i1];
			i1++;
		} else if (valid(i2, i2_limit) &&
				higher_or_invalid(array, i3, i3_limit, i2)) {
			aux_array[i_aux] = array[i2];
			i2++;
		} else {
			aux_array[i_aux] = array[i3];
			i3++;
		}
	}

	return aux_array;
}

int valid(int index, int limit) {
	return index < limit;
}

int higher_or_invalid(int array[], int value_index, int limit, int other_value_index) {
	return (array[value_index] > array[other_value_index]) || value_index >= limit;
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
	return current_size <= total_size / ((number_of_process + 1) / 2);
}

int is_leaf3(int current_size, int total_size, int number_of_process) {
//	printf("current size: %d, total_size: %d, delta: %d\n", current_size, total_size, (total_size / number_of_process));
	return current_size < (total_size / number_of_process) * 2;
}
```

## bubblesort.c
```c
#include "bubblesort.h"

void
bubble_sort(int size, int* array)
{
	int holder, swap = 1;
	for (int i = 0; i < size-1 && swap; ++i)
	{
		swap = 0;
		int limit = size - i -1;
		for (int j = 0 ; j < limit; ++j)
		{
			int current = array[j];
			int next = array[j + 1];
			if (current > next)
			{
				holder     = current;
				array[j]   = next;
				array[j+1] = holder;
				swap       = 1;
			}
		}
	}
}
```
