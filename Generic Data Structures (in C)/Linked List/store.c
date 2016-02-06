/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 4/3/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * A program that accepts a properly formatted file with data on
 * customers and their book purchases. Prints out a table of
 * customer ID numbers and the ID numbers of the books they purchased.
 * An error message will be given and the program will end returning one
 * if a bad filename is given, the number of customers designated is less
 * than 1, or the number of books purchased is less than 0.
 */

#include <stdlib.h>
#include <stdio.h>
#include "List.h"

int main ()
{
	int customers, purchases, inputTest, i, j, k, s;
	ListHndl* booksPurchased;
	char inputFileName[1024];
	FILE* dataFile;

	fprintf (stdout, "Please input the name of a properly formatted data file\n");
	fgets (inputFileName, 1024, stdin);

	for(j = 0; j < 1024; j++)
	{
		if (inputFileName[j] == '\n')
		{
			inputFileName[j] = '\0';
			break;
		}
	}

	dataFile = fopen (inputFileName, "r");

	if (dataFile == NULL)
	{
		fprintf (stdout, "Invalid filename\n");
		return 1;
	}

	inputTest = fscanf (dataFile, "%d", &customers);

	if (inputTest != 1)
	{
		fprintf (stderr, "Improperly formatted file\n");
		fclose (dataFile);
		return 1;
	}

	if (customers < 1)
	{
		fprintf (stderr, "Invalid input\n");
		fprintf (stderr, "Number of customers cannot be less than 1\n");
		fclose (dataFile);
		return 1;
	}

	booksPurchased = (ListHndl*)malloc ( (sizeof (ListHndl)) * customers );

	inputTest = fscanf (dataFile, "%d", &purchases);

	if (inputTest != 1)
	{
		fprintf (stderr, "Improperly formatted file\n");
		fclose (dataFile);
		free (booksPurchased);
		return 1;
	}

	if (purchases < 0)
	{
		fprintf (stderr, "Invalid input\n");
		fprintf (stderr, "Number of purchases cannot be negative\n");
		fclose (dataFile);
		free (booksPurchased);
		return 1;
	}

	for (i = 0; i < customers; i++)
	{
		booksPurchased[i] = NewList ();
	}

	for (k = 0; k < purchases; k++)
	{
		int customerNumber;
		long bookNumber;
		inputTest = fscanf (dataFile, "%d%ld", &customerNumber, &bookNumber);

		if (inputTest != 2)
		{
			fprintf (stderr, "Improperly formatted file\n");
			fclose (dataFile);
			for (s = 0; s < customers; s++)
			{
				FreeList (booksPurchased[s]);
			}
			free (booksPurchased);
			return 1;
		}

		if (customerNumber < 1 || customerNumber > customers)
		{
			fprintf (stderr, "Invalid customer number\n");
		}
		else
		{
			if (IsEmpty (booksPurchased[customerNumber - 1]))
			{
				InsertAtFront (booksPurchased[customerNumber - 1], bookNumber);
			}
			else
			{
				InsertionSort (booksPurchased[customerNumber - 1], bookNumber);
			}
		}

		
	}

	fprintf (stdout, "customer#   books purchased\n");

	for (s = 0; s < customers; s++)
	{
		fprintf (stdout, "%d  ", (s + 1));

		if(!IsEmpty (booksPurchased[s]))
		{
			MoveFirst (booksPurchased[s]);
		}

		while (!OffEnd(booksPurchased[s]))
		{
			fprintf (stdout, " ");
			PrintCurrent (stdout, booksPurchased[s]);
			MoveNext (booksPurchased[s]);
		}

		fprintf (stdout, "\n");

		FreeList (booksPurchased[s]);
	}

	fclose (dataFile);
	free (booksPurchased);
	return 0;
}
