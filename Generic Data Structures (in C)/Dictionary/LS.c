/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 5/23/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * A driver program for the Dictionary ADT that accepts a formatted
 * files as command line arguments. One of the files is read from to
 * fill the Dictionary ADT with book names and the library IDs of the
 * libraries at which each book is stored. After the dictionary is 
 * filled the second file is read from and books listed in the second 
 * file are searched for one by one. If a book search is successful 
 * a list of all the library IDs linked to the searched book will be
 * printed on a new line. If a book search fails due to the book not
 * being in the Dictionary a blank new line is printed.
 */

#include <stdlib.h>
#include <stdio.h>
#include "Dictionary.h"

/*
 * Preconditions: The given filenames must exist
 *                The files must be properly formatted
 *
 * Accepts two formatted data files. The first file should contain
 * information on how large a dictionary should be made to store the 
 * book information, and a list of book names with the corresponding 
 * Library ID's at which they are located. The second file should
 * contain a list of book names, with one name on each line. This
 * program will then look through the book names in the second file 
 * one by one and output the library IDs of the libraries at which 
 * each book is stored. Each list of library IDs will be printed a 
 * new line. If a book is searched for, but is not in any library a
 * blank line will be printed.
 */

int main ( int argc, char* argv [])
{
	int i, j;
	DictHndl library;
	int numBooks;
	int dictSize;
	int parser;
	int parseSave;
	int numSearches;
	FILE *libraryBooks;
	FILE *searchBooks;
	char buffer[1000];

	if (argc != 3)
	{
		fprintf (stderr, "usage: %s filename filename\n", argv[0]);
		return 1;
	}

	libraryBooks = fopen( argv[1], "r");
	searchBooks = fopen ( argv[2], "r");

	if (libraryBooks == 0 || searchBooks == 0)
	{
		fprintf (stderr, "File does not exist\n");
		return 1;
	}

	fgets (buffer, 1000, libraryBooks);

	parser = 0;
	while (buffer[parser] >= '0' && buffer[parser] <= '9')
	{
		parser++;
	}

	parseSave = parser;
	parser--;
	numBooks = 0;

	for (j = 1; parser >= 0; j = j * 10)
	{
		numBooks += (buffer[parser] - '0') * j;
		parser--;
	}

	parser = parseSave + 1;

	while (buffer[parser] >= '0' && buffer[parser] <= '9')
	{
		parser++;
	}

	dictSize = 0;
	for (j = 1; parser > parseSave; j = j * 10)
	{
		dictSize += (buffer[parser] - '0') * j;
		parser--;
	}

	library = NewDictionary (dictSize);

	for (i = 0; i < numBooks; i++)
	{
		int reparser = 0;
		int tempLocation = 0;
		parser = 0;
		fgets (buffer, 1000, libraryBooks);

		while (buffer[parser] != ',')
		{
			if (parser == 999)
			{
				fprintf (stderr, "Improper Library Data File\n");
				return 1;
			}
			parser++;
		}

		parseSave = parser;
		parser--;

		for (j = 1; parser >= 0; j = j * 10)
		{
			tempLocation += (buffer[parser] - '0') * j;
			parser--;
		}

		parser = parseSave;
		parser += 2;

		while (buffer[parser] != '\n' && 
			buffer[parser] != EOF && 
			buffer[parser] != '\0')
		{
			buffer[reparser] = buffer[parser];
			parser++;
			reparser++;
		}

		buffer[reparser] = '\0';
		InsertKey (library, buffer, tempLocation);
	}

	fgets (buffer, 1000, searchBooks);
	parser = 0;
	while (buffer[parser] >= '0' && buffer[parser] <= '9')
	{
		parser++;
	}

	parser--;
	numSearches = 0;
	for (j = 1; parser >= 0; j = j * 10)
	{
		numSearches += (buffer[parser] - '0') * j;
		parser--;
	}

	for (i = 0; i < numSearches; i++)
	{
		fgets (buffer, 1000, searchBooks);
		j = 0;
		while (buffer[j] != '\n' &&
			buffer[j] != '\0' &&
			buffer[j] != EOF)
		{
			j++;
		}
		buffer[j] = '\0';
		PrintKeyLocations (library, buffer);
	}

	FreeDictionary (library);
	fclose (libraryBooks);
	fclose (searchBooks);

	return 0;
}
