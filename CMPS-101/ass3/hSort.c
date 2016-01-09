/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 4/27/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * A sorting function that uses the functionality of the heap ADT
 * to sort an array of ints of length numKeys into ascending order.
 */

#include <stdlib.h>
#include "heap.h"

void HeapSort (int keys[], int numKeys)
{
	int i;
	heapHndl sortHeap;

	sortHeap = BuildHeap (numKeys, keys, numKeys);

	for (i = numKeys - 1; i >= 0; i--)
	{
		keys[i] = MaxValue (sortHeap);
		DeleteMax (sortHeap);
	}

	FreeHeap (sortHeap);
}
