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
 * HeapSort functions. The time it takes each algorithm to sort the 
 * array is calculate and printed out.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "heap.h"
#include "heapSort.h"
#include "insertionSort.h"

/*
 * Precondition: The given filename must exist
 *               The file must be properly formatted
 *
 * Accepts a properly formatted data file filled with integer values.
 * Uses insertion sort and then heap sort algorithms to sort two
 * different arrays filled with the ints from the data file. The time
 * it took for each algorithm to sort the array is printed out.
 */


int main ( int argc, char* argv [])
{
	clock_t startTime, duration;
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

	printf ("To sort an array of %d ints:\n", numKeys);

	startTime = clock();
	InsertionSort (insertionKeys, numKeys);
	duration = clock() - startTime;
	printf ("Insertion sort took %f seconds.\n", ((float) duration) / CLOCKS_PER_SEC);

	startTime = clock();
	HeapSort (heapKeys, numKeys);
	duration = clock() - startTime;
	printf ("Heap sort took %f seconds.\n", ((float) duration) / CLOCKS_PER_SEC);

	free (insertionKeys);
	free (heapKeys);
	fclose (file);

	return 0;
}
