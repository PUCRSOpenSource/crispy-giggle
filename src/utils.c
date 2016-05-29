#include "utils.h"

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
int *interleaving3(int array[], int size) {
	int *aux_array;
	int i1, i2, i3, i1_limit, i2_limit, i3_limit, i_aux;

	aux_array = calloc(size, sizeof(int));

	i1 = 0;
	i1_limit = size / 3;
	i2 = size / 3;
	i2_limit = 2 * size / 3;
	i3 = 2 * size / 3;
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
	return current_size <= (total_size / number_of_process);
}
