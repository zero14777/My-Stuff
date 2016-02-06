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
	long temp;

	testList = NewList ();

	if (IsEmpty (testList))
	{
		printf ("List is empty\n");
	}

	InsertAtFront (testList, 1);
	InsertAtBack (testList, 4);
	InsertionSort (testList, 2);
	MoveLast (testList);
	MovePrev (testList);
	MoveNext (testList);
	InsertBeforeCurrent (testList, 3);

	PrintList (stdout, testList);

	DeleteCurrent (testList);
	DeleteFirst (testList);
	DeleteLast (testList);
	MoveFirst (testList);

	PrintList (stdout, testList);

	if (AtFirst (testList))
	{
		printf ("Looking at current node\n");
	}

	if (AtLast (testList))
	{
		printf ("Looking at last node\n");
	}

	temp = GetCurrent (testList);
	InsertBeforeCurrent (testList, temp);
	temp = GetFirst (testList);
	InsertAtFront (testList, temp);
	temp = GetLast (testList);
	InsertAtBack (testList, temp);

	PrintList (stdout, testList);

	PrintCurrent(stdout, testList);

	printf ("\n");

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
