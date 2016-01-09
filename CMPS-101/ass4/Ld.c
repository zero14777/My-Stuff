/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 4/3/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * A test driver program for the List.c abstract data type. This
 * program creates a ListHndl and calls all of the functions in List.c 
 * at least once to test them. Some test output is printed and the List
 * is freed before the program ends.
 */

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "List.h"

int main ()
{
	ListHndl testList;
	int* test1 = malloc ( sizeof (int) * 2 );
	int* test2 = malloc ( sizeof (int) * 2 );
	int* test3 = malloc ( sizeof (int) * 2 );
	int* output;

	test1[0] = 1;
	test1[1] = 2;
	test2[0] = 2;
	test2[1] = 3;
	test3[0] = 3;
	test3[1] = 4;

	testList = NewList ();

	if (IsEmpty (testList))
	{
		printf ("List is empty\n");
	}

	InsertAtFront (testList, test1);
	InsertAtBack (testList, test2);
	MoveLast (testList);
	MovePrev (testList);
	MoveNext (testList);
	InsertBeforeCurrent (testList, test3);

	MovePrev (testList);
	output = (int *) GetFirst (testList);
	printf ("%d %d\n", output[0], output[1]);
	output = (int *) GetCurrent (testList);
	printf ("%d %d\n", output[0], output[1]);
	output = (int *) GetLast (testList);
	printf ("%d %d\n", output[0], output[1]);
	MoveNext (testList);

	DeleteCurrent (testList);
	DeleteFirst (testList);
	printf ("got here1\n");
	fflush (stdout);
	MoveFirst (testList);
	printf ("got here2\n");
	fflush (stdout);

	if (AtFirst (testList))
	{
		printf ("Looking at first node\n");
	}

	if (AtLast (testList))
	{
		printf ("Looking at last node\n");
	}

	MakeEmpty (testList);

	if (IsEmpty (testList))
	{
		printf ("List is empty\n");
	}

	if (OffEnd (testList))
	{
		printf ("There is no current node\n");
	}

	FreeList (testList);

	return (0);
}
