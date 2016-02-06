// $Id: hashset.h,v 1.2 2013-03-04 20:49:14-08 - - $

#ifndef __HASHSET_H__
#define __HASHSET_H__

#include <stdbool.h>

typedef struct hashset hashset;

//
// Create a new hashset with a default number of elements.
//
hashset *new_hashset (void);

// Returns the load value for the given hashset

size_t hashsetload(hashset *);

//
// Frees the hashset, and the words it points at.
//
void free_hashset (hashset *);

// Prints out information about the chains of the given hashset
// and its related clusters.

void hashprint (hashset *);

// Prints out the the hashset along with printing the hashvalue of
// each inserted string.

void hashprintmore (hashset *);

//
// Inserts a new string into the hashset.
//
void put_hashset (hashset *, char*);

//
// Looks up the string in the hashset and returns true if found,
// false if not found.
//
bool has_hashset (hashset *, char*);

#endif

