/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 4/26/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * This .h file defines the functions of the heap ADT written in heap.c
 */

#ifndef _HEAP_H_
#define _HEAP_H_

typedef int bool;
#define true 1
#define false 0

typedef struct HeapStruct* heapHndl;

/*
 * Allocates space in memory for a new list
 */

heapHndl NewHeap (int maxSize);

/*
 * Precondition: The max size of the heap can't be larger than the number
 *               of ints to be inserted.
 *
 * Creates heap and fills it with a set of given ints.
 */

heapHndl BuildHeap (int maxSize, int* data, int numData);

/*
 * Precondition: Heap was created with NewHeap.
 *
 * Frees the space in memory allocated for the heap.
 */

void FreeHeap (heapHndl heap);

/*
 * Precondition: heap was created with NewHeap.
 *               All nodes in the heap below the problem parent are ordered
 *               properly.
 *
 * Sinks a problem parent value to a lower position in the heap where it's
 * relationships do not conflict with the heaps ordering.
 */

void Sink (heapHndl heap, int arrayPosition);

/*
 * Precondition: heap was created with NewHeap.
 *               All other nodes in the heap are ordered properly.
 *
 * Swims a problem child value to a higher position in the heap where it's
 * relationships do not conflict with the heaps ordering.
 */

void Swim (heapHndl heap, int arrayPosition);

/*
 * Precondition: Heap was created with NewHeap.
 *
 * Returns true if the current size of the heap is the max size of the heap.
 * Returns false if the current size of heap does not equal the max size.
 */

bool IsFull (heapHndl heap);

/*
 * Precondition: Heap was created with NewHeap.
 *
 * Returns true if the current size of the heap is 0.
 * Returns false if the current size of the heap is not 0.
 */

bool IsEmpty (heapHndl heap);

/*
 * Preconditions: Heap was created with NewHeap.
 *                Heap's current size is at least 1.
 *
 * Returns the value in the heap with the highest Priority.
 */

int MaxValue (heapHndl heap);

/*
 * Preconditions: Heap was created with NewHeap.
 *                Heap's current size is at least 1.
 *
 * Deletes the highest priority int from the heap, then reorders the heap.
 */

void DeleteMax (heapHndl heap);

/*
 * Preconditions: Heap was preated with NewHeap.
 *                Heap's current size is at least 1 less than the heaps
 *                max size.
 *
 * Postconditions: The heap cannot be empty.
 *
 * Inserts a new int into the heap, then reorders the heap.
 */

void Insert (heapHndl heap, int priority);

#endif
