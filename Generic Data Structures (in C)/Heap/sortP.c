/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 4/26/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * A driver program for the heap ADT that accepts a properly formatted
 * file as a command line argument. The file is opened and integers
 * in the file are read into two identical arrays. The arrays are
 * both sorted into ascending order using the InsertionSort and 
 * HeapSort functions. The sorted arrays are printed out.
 */

#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "heapSort.h"
#include "insertionSort.h"

/*
 * Precondition: The keys array has at least numKeys number of keys
 *
 * uses a for loop to iterate through an array of ints and check
 * for ascending order. If the array of ints is not in ascending
 * order the function returns false. If the function os properly
 * ordered the function returns true.
 */

bool Sorted (int keys[], int numKeys)
{
	int i;

	for (i = 0; i < numKeys; i++)
	{
		if (i != (numKeys - 1) &&
		    keys[i] > keys[i + 1])
		{
			return false;
		}
	}

	return true;
}

/*
 * Precondition: The keys array has at least numKeys number of keys
 *
 * This function iterates through the array and prints out each element
 * of the array on a new line.
 */

void PrintArray (int keys[], int numKeys)
{
	int i;

	for (i = 0; i < numKeys; i++)
	{
		printf("%d\n", keys[i]);
	}
}

/*
 * Preconditions: The given filename must exist
 *                The file must be properly formatted
 *
 * Accepts a properly formatted data file filled with integer values.
 * Uses insertion sort and then heap sort algorithms to sort two 
 * different arrays filled with the ints from the data file. The sorted
 * arrays are then printed with each element being printed on a new line.
 */

int main ( int argc, char* argv [])
{
	int *insertionKeys;
	int *heapKeys;
	int numKeys, temp, i;
	FILE *file;

	if (argc != 2)
	{
		fprintf (stderr, "usage: %s filename\n", argv[0]);
		return 1;
	}

	file = fopen( argv[1], "r");

	if (file == 0)
	{
		fprintf (stderr, "File does not exist\n");
		return 1;
	}

	fscanf (file, "%d", &numKeys);

	insertionKeys = malloc ( sizeof (int) * numKeys );
	heapKeys = malloc ( sizeof (int) * numKeys );

	for (i = 0; i < numKeys; i++)
	{
		fscanf (file, "%d", &temp);
		insertionKeys[i] = temp;
		heapKeys[i] = temp;
	}

	InsertionSort (insertionKeys, numKeys);
	HeapSort (heapKeys, numKeys);

	if (!Sorted (insertionKeys, numKeys) ||
	    !Sorted (heapKeys, numKeys))
	{
		free (insertionKeys);
		free (heapKeys);
		fclose (file);
		fprintf (stderr, "sorting failed\n");
		return 1;
	}

	printf ("Insertion Sorted array:\n");
	PrintArray (insertionKeys, numKeys);

	printf ("\nHeap Sorted array:\n");
	PrintArray (heapKeys, numKeys);

	free (insertionKeys);
	free (heapKeys);
	fclose (file);

	return 0;
}
