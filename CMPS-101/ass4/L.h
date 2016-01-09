/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 5/24/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * This .h file corresponds to the List.c file.
 */

#ifndef _LIST_H_
#define _LIST_H_

typedef struct NodeStruct* Node;

typedef struct ListStruct* ListHndl;

/*
 * Allocates space in memory for a new list
 */

ListHndl NewList ();

/*
 * Creates a new node with a given value that can be put into a list
 */

Node NewNode (void* data);

/*
 * Precondition: List has been created with NewList.
 *
 * Frees the memory allocation of every node in a list then frees the memory
 * allocated for the given list.
 */

void FreeList (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *
 * Returns true if the list is empty. If the list is not empty it returns false.
 */

int IsEmpty (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *
 * Returns true if current equals NULL. If current does not equal NULL OffEnd
 * returns false.
 */

int OffEnd (ListHndl list);

/*
 * Preconditions: List has been created with NewList.
 *
 * Returns true if current equals first, and current does not equal NULL.
 * Returns false in all other situations.
 */

int AtFirst (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *
 * Returns true if current equals last, and current does not equal NULL.
 * Returns false in all other situations.
 */

int AtLast (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *               The list must not be empty.
 *
 * Returns the first void * pointer in the list.
 */

void* GetFirst (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *               The list must not be empty.
 *
 * Returns the last void * pointer in the list.
 */

void* GetLast (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *               Current must not equal NULL.
 *
 * Returns the void * pointer of the current node.
 */

void* GetCurrent (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *
 * Postcondition: The list is empty.
 *                Current is equal to NULL.
 *
 * Removes all nodes from the list.
 */

void MakeEmpty (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *               The list must not be empty.
 *
 * Postcondition: Current cannot equal NULL.
 *
 * Sets the current node to be the first node in the list.
 */

void MoveFirst (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *               The list must not be empty.
 *
 * Postcondition: Current cannot equal NULL.
 *
 * Sets the current node to be the last node in the list.
 */

void MoveLast (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *               Current cannot equal NULL.
 *
 * Postcondition: Current equals NULL if current was at first before.
 *
 * Sets the current node to be the previous node in the list.
 */

void MovePrev (ListHndl list);

/*
 * Precondition: List has been created with NewList.
 *               Current cannot equal NULL.
 *
 * Postcondition: Current equals NULL if current was at last before.
 *
 * Sets the current node to be the next node in the list.
 */

void MoveNext (ListHndl list);

/*
 * Preconditions: List has been created with NewList.
 *
 * Postconditions: The list cannot be empty.
 *
 * Inserts a new node with the given void * pointer at the front of
 * the list.
 */

void InsertAtFront (ListHndl list, void* data);

/*
 * Preconditions: List has been created with NewList.
 *
 * Postconditions: The list cannot be empty.
 *
 * Inserts a new node with the given void * pointer at the back of
 * the list.
 */

void InsertAtBack (ListHndl list, void* data);

/*
 * Preconditions: List has been created with NewList.
 *                Current cannot equal NULL.
 *
 * Postconditions: The list cannot be empty.
 *                 Current cannot equal the first node in the list.
 *
 * Inserts a new node with the given void * pointer before the current
 * node in the list.
 */

void InsertBeforeCurrent (ListHndl list, void* data);

/*
 * Preconditions: List has been created with NewList.
 *                The list cannot be empty.
 *
 * Removes and frees the node at the front of the list.
 */

void DeleteFirst (ListHndl list);

/*
 * Preconditions: List has been created with NewList.
 *                The list cannot be empty.
 *
 * Removes and frees the node at the back of the list.
 */

void DeleteLast (ListHndl list);

/*
 * Preconditions: List has been created with NewList.
 *                The list cannot be empty.
 *                Current must not equal NULL.
 *
 * Postconditions: Current equals NULL.
 *
 * Removes and frees the current node of the list.
 */

void DeleteCurrent (ListHndl list);

#endif
