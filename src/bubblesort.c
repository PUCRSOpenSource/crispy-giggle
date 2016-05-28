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
