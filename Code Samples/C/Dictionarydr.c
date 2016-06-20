/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 5/24/2014
 * 
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 * 
 * A simple driver program that tests the functionality of the 
 * Dictionary ADT.
 */

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "Dictionary.h"
#include "List.h"

int main ()
{
	DictHndl testDict;
	ListHndl testLookUp;

	testDict = NewDictionary (10);

	InsertKey (testDict, "cat", 123);

	InsertKey (testDict, "dog", 234);

	InsertKey (testDict, "fog", 2345);

	InsertKey (testDict, "fog", 245);

	testLookUp = LookUp (testDict, "cat");

	printf ("%d\n" , *(int*) GetFirst (testLookUp));

	PrintKeyLocations (testDict, "fog");

	if (IsIn (testDict, "dog"))
	{
		printf ("dog is in the table\n");
	}

	DeleteKey (testDict, "cat");

	DeleteKey (testDict, "dog");

	if (IsIn (testDict, "dog"))
	{
		printf ("dog is still in the table\n");
	}

	FreeDictionary (testDict);

	return (0);
}
