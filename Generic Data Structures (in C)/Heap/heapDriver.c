/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 4/26/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * This is a test driver program for the heap ADT implemented by heap.c
 * and heap.h. Each function of the heap ADT is called at least once in this
 * program.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "heap.h"

int main ()
{
	int heapSize = 12;
	int intArray[4] = {4, 6, 2, 8};
	heapHndl testHeap, builtHeap;

	testHeap = NewHeap (heapSize);

	if (IsEmpty (testHeap))
	{
		printf ("working\n");
	}

	Insert (testHeap, 9);
	Insert (testHeap, 3);
	Insert (testHeap, 5);
	Insert (testHeap, 2);
	Insert (testHeap, 32);
	Insert (testHeap, 6);
	Insert (testHeap, 1);
	Insert (testHeap, 43);
	Insert (testHeap, 23);
	Insert (testHeap, 42);
	Insert (testHeap, 65);
	Insert (testHeap, 57);
	if (IsFull (testHeap))
	{
		printf ("heap currently full\n");
	}
	DeleteMax (testHeap);

	builtHeap = BuildHeap (5, intArray, 4);

	printf ("%d\n", MaxValue (testHeap));
	printf ("%d\n", MaxValue (builtHeap));

	FreeHeap (testHeap);
	FreeHeap (builtHeap);

	return 0;
}
