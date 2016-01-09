/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 5/24/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * This .h file corresponds to the Dictionary.c file.
 */


#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include "List.h"

typedef struct DictionaryStruct* DictHndl;

typedef struct KeyLocationStruct* KeyNode;

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

int HashFunction (char* key, int size);

/*
 * Precondition: The given size is greater than 0
 *
 * Allocates the space in memory for a new dictionary and returns the handle
 * to the newly created dictionary.
 */

DictHndl NewDictionary (int size);

/*
 * Allocates the space in memory for a new KeyNode and returns the handle
 * to the newly created KeyNode.
 */

KeyNode NewKeyNode (char* key, int location);

/*
 * Precondition: The given node was created with the NewKeyNode function.
 *
 * Adds a new location value onto the locations list of an existing KeuNode.
 */

void AddLocationToNode (KeyNode node, int location);

/*
 * Precondition: The given dictionary was created with the NewDictionary
 *               function.
 *
 * Frees the space in memory allocated to the given dictionary.
 */

void FreeDictionary (DictHndl dict);

/*
 * Precondition: The given dictionary was created with the NewDictionary
 *               function.
 *
 * Adds the given location onto the KeyNode with the given key on the
 * given dictionary. If the dictionary does not have a KeyNode with the
 * given key a new KeyNode with the given key and location is created and
 * inserted into the dictionary.
 */

void InsertKey (DictHndl dict, char* key, int location);

/*
 * Precondition: The given dictionary was created with the NewDictionary
 *               function.
 *
 * Deletes the key and all its' related information from the dictionary.
 */

void DeleteKey (DictHndl dict, char* key);

/*
 * Precondition: The given dictionary was created with the NewDictionary
 *               function.
 *
 * Looks for and returns the list of locations stored in the KeyNode with
 * the given key. If no such KeyNode exists NULL is returned.
 */

ListHndl LookUp (DictHndl dict, char* key);

/*
 * Precondition: The given dictionary was created with the NewDictionary
 *               function.
 *
 * Checks to see if a KeyNode exists in the given dictionary that holds the
 * given key. If such a KeyNode exists 1 is returned. If there is no such
 * KeyNode 0 is returned.
 */

int IsIn (DictHndl dict, char* key);

/*
 * Preconditions: The given dictionary was created with the NewDictionary
 *                function.
 *
 * Looks for and prints out the locations stored in the dictionary connected
 * the the given key. A single space is printed between each location, and a
 * new line character is printed regardless of whether any locations are found.
 */

void PrintKeyLocations (DictHndl dict, char* key);

#endif
