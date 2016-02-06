/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 4/27/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * A simple insertion sort function that sorts an array of ints with
 * numKeys ints into ascending order.
 */

#include <stdlib.h>

void InsertionSort (int keys[], int numKeys)
{
	int i, j, temp;

	for (i = 1; i < numKeys; i++)
	{
		j = i;
		while (j > 0 && keys[j] < keys[(j - 1)])
		{
			temp = keys[(j - 1)];
			keys[(j - 1)] = keys[j];
			keys[j] = temp;
			j--;
		}
	}
}
