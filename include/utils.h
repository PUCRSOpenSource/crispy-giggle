#pragma once

#include <stdio.h>
#include <stdlib.h>

int *interleaving(int array[], int size);
int left_child(int index);
int right_child(int index);
int parent(int index);
int is_leaf(int current_size, int total_size, int number_of_process);
int is_leaf3(int current_size, int total_size, int number_of_process);
int *interleaving3(int array[], int size, int slice_size);
