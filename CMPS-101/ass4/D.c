/*
 *
 *
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "List.h"
#include "Dictionary.h"

struct DictionaryStruct
{
	int size;
	ListHndl* table;
} DictionaryStruct;

struct KeyLocationStruct
{
	char* key;
	ListHndl locations;
} KeyLocationStruct;

/*
 * Precondition: The given size is greater than 0.
 *
 * Postcondition: The returned value is greater than or equal to 0
 *                and less than size.
 *
 * This function uses a hash function taken from the sdbm database library
 * to find the slot in the dictionary of the given size that the given key
 * should hash into.
 */

int HashFunction (char* key, int size)
{
	unsigned long hash = 0;
	int c, hashSlot;

	assert (size > 0);

	while ((c = *key++))
	{
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	hashSlot = (hash % size);

	assert (hashSlot >= 0);
	assert (hashSlot < size);

	return hashSlot;
}

/*
 * Precondition: The given size is greater than 0
 *
 * Allocates the space in memory for a new dictionary and returns the handle
 * to the newly created dictionary.
 */

DictHndl NewDictionary (int size)
{
	int i;
	DictHndl tempDict;

	assert (size > 0);

	tempDict = malloc ( sizeof (DictionaryStruct) );
	tempDict->size = size;
	tempDict->table = malloc ( (sizeof (ListHndl)) * size );
	for (i = 0; i < size; i++)
	{
		tempDict->table[i] = NULL;
	}
	return (tempDict);
}

/*
 * Allocates the space in memory for a new KeyNode and returns the handle
 * to the newly created KeyNode.
 */

KeyNode NewKeyNode (char* key, int location)
{
	int* locationPointer = malloc ( sizeof (int) );
	KeyNode temp = malloc ( sizeof (KeyLocationStruct) );
	char* dictKey = malloc ( sizeof (char) * (strlen (key) + 1) );
	strcpy (dictKey, key);

	*locationPointer = location;
	temp->key = dictKey;
	temp->locations = NewList ();
	InsertAtFront (temp->locations, locationPointer);

	return temp;
}

/*
 * Precondition: The given node was created with the NewKeyNode function.
 *
 * Adds a new location value onto the locations list of an existing KeuNode.
 */

void AddLocationToNode (KeyNode node, int location)
{
	assert (node != NULL);

	MoveFirst (node->locations);
	while (!OffEnd (node->locations))
	{
		int* temp = (int*) GetCurrent (node->locations);

		if (*temp == location)
		{
			break;
		}

		MoveNext (node->locations);
	}

	if(OffEnd (node->locations))
	{
		int* newLocation = malloc ( sizeof (int) );
		*newLocation = location;
		InsertAtBack (node->locations, newLocation);
	}
}

/*
 * Precondition: The given dictionary was created with the NewDictionary
 *               function.
 *
 * Frees the space in memory allocated to the given dictionary.
 */

void FreeDictionary (DictHndl dict)
{
	int i;

	assert (dict != NULL);

	for (i = 0; i < dict->size; i++)
	{
		if (dict->table[i] != NULL)
		{
			MoveFirst (dict->table[i]);
			while (!OffEnd (dict->table[i]))
			{
				KeyNode temp = (KeyNode) GetCurrent (dict->table[i]);
				free (temp->key);
				MoveFirst (temp->locations);
				FreeList (temp->locations);
				MoveNext (dict->table[i]);
			}
			FreeList (dict->table[i]);
		}
	}

	free (dict->table);
	free (dict);
}

/*
 * Precondition: The given dictionary was created with the NewDictionary
 *               function.
 *
 * Adds the given location onto the KeyNode with the given key on the 
 * given dictionary. If the dictionary does not have a KeyNode with the 
 * given key a new KeyNode with the given key and location is created and 
 * inserted into the dictionary.
 */

void InsertKey (DictHndl dict, char* key, int location)
{
	int slot;
	int keyFound = 0;

	assert (dict != NULL);

	slot = HashFunction (key, dict->size);

	if (dict->table[slot] == NULL)
	{
		dict->table[slot] = NewList ();
	}

	if (!IsEmpty (dict->table[slot]))
	{
		MoveFirst (dict->table[slot]);
		while (!OffEnd (dict->table[slot]))
		{
			KeyNode temp = (KeyNode) GetCurrent (dict->table[slot]);

			if ( 0 == strcmp (key, temp->key) )
			{
				AddLocationToNode (temp, location);
				keyFound = 1;
				break;
			}

			MoveNext (dict->table[slot]);
		}
	}

	if (keyFound == 0)
	{
		InsertAtBack (dict->table[slot], NewKeyNode (key, location));
	}
}

/*
 * Precondition: The given dictionary was created with the NewDictionary 
 *               function.
 *
 * Deletes the key and all its' related information from the dictionary.
 */

void DeleteKey (DictHndl dict, char* key)
{
	int slot;

	assert (dict != NULL);

	slot = HashFunction (key, dict->size);

	assert (!IsEmpty(dict->table[slot]));

	MoveFirst (dict->table[slot]);

	while (!OffEnd (dict->table[slot]))
	{
		KeyNode temp = (KeyNode) GetCurrent (dict->table[slot]);
		if ( 0 == strcmp (key, temp->key) )
		{
			free (temp->key);
			FreeList (temp->locations);
			DeleteCurrent (dict->table[slot]);
			break;
		}

		MoveNext (dict->table[slot]);
	}

	if (IsEmpty (dict->table[slot]))
	{
		FreeList (dict->table[slot]);
	}

	dict->table[slot] = NULL;
}

/*
 * Precondition: The given dictionary was created with the NewDictionary
 *               function.
 *
 * Looks for and returns the list of locations stored in the KeyNode with
 * the given key. If no such KeyNode exists NULL is returned.
 */

ListHndl LookUp (DictHndl dict, char* key)
{
	int slot;

	assert (dict != NULL);

	slot = HashFunction (key, dict->size);

	if (dict->table[slot] == NULL)
	{
		return NULL;
	}

	MoveFirst (dict->table[slot]);

	while (!OffEnd (dict->table[slot]))
	{
		KeyNode temp = (KeyNode) GetCurrent (dict->table[slot]);

		if ( 0 == strcmp (key, temp->key) )
		{
			return (temp->locations);
		}

		MoveNext (dict->table[slot]);
	}

	return NULL;
}

/*
 * Precondition: The given dictionary was created with the NewDictionary
 *               function.
 *
 * Checks to see if a KeyNode exists in the given dictionary that holds the
 * given key. If such a KeyNode exists 1 is returned. If there is no such
 * KeyNode 0 is returned.
 */

int IsIn (DictHndl dict, char* key)
{
	int slot;

	assert (dict != NULL);

	slot = HashFunction (key, dict->size);

	if (dict->table[slot] == NULL)
	{
		return 0;
	}

	MoveFirst (dict->table[slot]);

	while (!OffEnd (dict->table[slot]))
	{
		KeyNode temp = (KeyNode) GetCurrent (dict->table[slot]);

		if ( 0 == strcmp (key, temp->key) )
		{
			return 1;
		}

		MoveNext (dict->table[slot]);
	}

	return 0;
}

/*
 * Preconditions: The given dictionary was created with the NewDictionary
 *                function.
 *
 * Looks for and prints out the locations stored in the dictionary connected
 * the the given key. A single space is printed between each location, and a
 * new line character is printed regardless of whether any locations are found.
 */

void PrintKeyLocations (DictHndl dict, char* key)
{
	int slot;

	assert (dict != NULL);

	slot = HashFunction (key, dict->size);

	if (dict->table[slot] == NULL)
	{
		printf ("\n");
		return;
	}

	if (IsEmpty (dict->table[slot]))
	{
		printf ("\n");
		return;
	}

	MoveFirst (dict->table[slot]);

	while (!OffEnd (dict->table[slot]))
	{
		KeyNode temp = (KeyNode) GetCurrent (dict->table[slot]);

		if (0 == strcmp (key, temp->key))
		{
			MoveFirst (temp->locations);

			while (!OffEnd (temp->locations))
			{
				int tempLocation = *(int *) GetCurrent (temp->locations);
				printf ("%d ", tempLocation);
				MoveNext (temp->locations);
			}

			break;
		}

		MoveNext (dict->table[slot]);
	}

	printf ("\n");
}
