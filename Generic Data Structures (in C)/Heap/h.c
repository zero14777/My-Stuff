/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 4/26/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * This is the implementation of a heap Abstract Data Type that uses
 * an int array to keep track of the heap structure. The int array elements
 * that are greater have more priority.
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "heap.h"

struct HeapStruct
{
	int maxSize;
	int currSize;
	int* heapArray;
} HeapStruct;

/*
 * Allocates space in memory for a new heap.
 */

heapHndl NewHeap (int maxSize)
{
	heapHndl tempHeap;
	tempHeap = malloc ( sizeof (struct HeapStruct) );
	tempHeap->maxSize = maxSize;
	tempHeap->currSize = 0;
	tempHeap->heapArray = malloc ( sizeof (int) * (maxSize + 1) );
	memset (tempHeap->heapArray, 0, (maxSize + 1));
	return tempHeap;
}

/*
 * Precondition: The max size of the heap can't be larger than the number
 *               of ints to be inserted.
 *
 * Creates heap and fills it with a set of given ints.
 */

heapHndl BuildHeap (int maxSize, int* data, int numData)
{
	int i;
	heapHndl tempHeap;

	assert (numData <= maxSize);

	tempHeap = NewHeap (maxSize);
	
	for (i = 0; i < numData; i++)
	{
		Insert (tempHeap, data[i]);
	}

	return tempHeap;
}

/*
 * Precondition: Heap was created with NewHeap.
 *
 * Frees the space in memory allocated for the heap.
 */

void FreeHeap (heapHndl heap)
{
	assert (heap != NULL);

	free (heap->heapArray);
	free (heap);
}

/*
 * Precondition: heap was created with NewHeap.
 *               All nodes in the heap below the problem parent are ordered
 *               properly.
 *
 * Sinks a problem parent value to a lower position in the heap where it's
 * relationships do not conflict with the heaps ordering.
 */

void Sink (heapHndl heap, int arrayPosition)
{
	int firstChild, secondChild, parent, highestPriority, temp;

	assert (heap != NULL);

	parent = arrayPosition;
	firstChild = (parent * 2);
	secondChild = (parent * 2) + 1;

	if (firstChild > heap->currSize)
	{
		return;
	}

	if (heap->heapArray[parent] > heap->heapArray[firstChild])
	{
		highestPriority = parent;
	}
	else
	{
		highestPriority = firstChild;
	}

	if (secondChild <= heap->currSize)
	{
		if (heap->heapArray[secondChild] > heap->heapArray[highestPriority])
		{
			highestPriority = secondChild;
		}
	}

	while (parent != highestPriority)
	{
		temp = heap->heapArray[highestPriority];
		heap->heapArray[highestPriority] = heap->heapArray[parent];
		heap->heapArray[parent] = temp;
		parent = highestPriority;
		firstChild = highestPriority * 2;
		secondChild = (highestPriority * 2) + 1;

		if (firstChild > heap->currSize)
		{
			break;
		}

		if (heap->heapArray[parent] > heap->heapArray[firstChild])
		{
			highestPriority = parent;
		}
		else
		{
			highestPriority = firstChild;
		}

		if (secondChild <= heap->currSize)
		{
			if (heap->heapArray[secondChild] > heap->heapArray[highestPriority])
			{
				highestPriority = secondChild;
			}
		}
	}
}

/*
 * Precondition: heap was created with NewHeap.
 *               All other nodes in the heap are ordered properly.
 *
 * Swims a problem child value to a higher position in the heap where it's
 * relationships do not conflict with the heaps ordering.
 */

void Swim (heapHndl heap, int arrayPosition)
{
	int child, parent;

	child = arrayPosition;
	parent = child/2;

	while (heap->heapArray[parent] < heap->heapArray[child] &&
	       parent > 0)
	{
		int temp = heap->heapArray[parent];
		heap->heapArray[parent] = heap->heapArray[child];
		heap->heapArray[child] = temp;
		child = parent;
		parent = child/2;
	}
}

/*
 * Precondition: Heap was created with NewHeap.
 *
 * Returns true if the current size of the heap is the max size of the heap.
 * Returns false if the current size of heap does not equal the max size.
 */

bool IsFull (heapHndl heap)
{
	assert (heap != NULL);

	if (heap->maxSize == heap->currSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * Precondition: Heap was created with NewHeap.
 *
 * Returns true if the current size of the heap is 0.
 * Returns false if the current size of the heap is not 0.
 */

bool IsEmpty (heapHndl heap)
{
	assert (heap != NULL);

	if (heap->currSize == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * Preconditions: Heap was created with NewHeap.
 *                Heap's current size is at least 1.
 *
 * Returns the value in the heap with the highest Priority.
 */

int MaxValue (heapHndl heap)
{
	assert (heap != NULL);
	assert (heap->currSize > 0);

	return heap->heapArray[1];
}

/*
 * Preconditions: Heap was created with NewHeap.
 *                Heap's current size is at least 1.
 *
 * Deletes the highest priority int from the heap, then reorders the heap.
 */

void DeleteMax (heapHndl heap)
{
	assert (heap != NULL);
	assert (heap->currSize > 0);

	heap->heapArray[1] = heap->heapArray[heap->currSize];
	heap->heapArray[heap->currSize] = 0;
	heap->currSize--;

	Sink (heap, 1);
}

/*
 * Preconditions: Heap was preated with NewHeap.
 *                Heap's current size is at least 1 less than the heaps 
 *                max size.
 *
 * Postconditions: The heap cannot be empty.
 *
 * Inserts a new int into the heap, then reorders the heap.
 */

void Insert (heapHndl heap, int priority)
{
	assert (heap != NULL);
	assert (heap->currSize < heap->maxSize);

	heap->heapArray[heap->currSize + 1] = priority;
	heap->currSize++;

	Swim (heap, heap->currSize);

	assert (!IsEmpty (heap));
}
