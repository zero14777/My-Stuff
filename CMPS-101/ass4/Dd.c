/*
 *
 *
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "Dictionary.h"
#include "List.h"

int main ()
{
	DictHndl testHash;
	ListHndl testLookUp;

	testHash = NewDictionary (10);

	InsertKey (testHash, "cat", 123);

	InsertKey (testHash, "dog", 234);

	InsertKey (testHash, "fog", 2345);

	InsertKey (testHash, "fog", 245);

	testLookUp = LookUp (testHash, "fog");

	MoveFirst (testLookUp);
	while (!OffEnd (testLookUp))
	{
		int temp = *(int*) GetCurrent (testLookUp);
		printf ("%d ", temp);
		MoveNext (testLookUp);
	}

	printf ("\n");

	if (IsIn (testHash, "dog"))
	{
		printf ("dog is in the table\n");
	}

	DeleteKey (testHash, "cat");

	DeleteKey (testHash, "dog");

	if (IsIn (testHash, "dog"))
	{
		printf ("dog is still in the table\n");
	}

	testLookUp = LookUp (testHash, "dog");

	FreeDictionary (testHash);

	return (0);
}
