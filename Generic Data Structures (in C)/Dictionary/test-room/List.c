/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 5/24/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * A doubly linked list abstract data type. A list is made up of
 * nodes that store the previous node in the list, the next node in
 * the list, and a generic void * pointer. Each list has pointers
 * to the first, last, and current nodes of the list.
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"

struct NodeStruct
{
	void* data;
	Node next;
	Node prev;
} NodeStruct;

struct ListStruct
{
	Node first;
	Node last;
	Node current;
} ListStruct;

int NumLists = 0;

/*
 * Allocates space in memory for a new list
 */

ListHndl NewList ()
{
	ListHndl tempList;
	tempList = malloc ( sizeof (ListStruct) );
	tempList->first = NULL;
	tempList->last = NULL;
	tempList->current = NULL;
	NumLists = NumLists + 1;
	return (tempList);
}

/*
 * Creates a new node with a given value that can be put into a list
 */

static Node NewNode (void* data)
{
	Node tempNode = malloc ( sizeof (NodeStruct) );
	tempNode->data = data;
	tempNode->next = NULL;
	tempNode->prev = NULL;
	return tempNode;
}

/*
 * Precondition: List has been created with NewList.
 *
 * Frees the memory allocation of every node in a list then frees the memory
 * allocated for the given list.
 */

void FreeList (ListHndl list)
{
	Node currNode;
	Node freeNode;

	assert (list != NULL);

	currNode = list->first;

	while (currNode != NULL)
	{
		freeNode = currNode;
		currNode = currNode->next;
		free (freeNode->data);
		free (freeNode);
	}

	free (list);
}

/*
 * Precondition: List has been created with NewList.
 *
 * Returns true if the list is empty. If the list is not empty it returns false.
 */

int IsEmpty (ListHndl list)
{
	assert (list != NULL);

	if (list->first == NULL && list->last == NULL && list->current == NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * Precondition: List has been created with NewList.
 *
 * Returns true if current equals NULL. If current does not equal NULL OffEnd
 * returns false.
 */

int OffEnd (ListHndl list)
{
	assert (list != NULL);

	if (list->current == NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * Preconditions: List has been created with NewList.
 *
 * Returns true if current equals first, and current does not equal NULL.
 * Returns false in all other situations.
 */

int AtFirst (ListHndl list)
{
	assert (list != NULL);

	if (!OffEnd (list) && list->first == list->current)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * Precondition: List has been created with NewList.
 *
 * Returns true if current equals last, and current does not equal NULL.
 * Returns false in all other situations.
 */

int AtLast (ListHndl list)
{
	assert (list != NULL);

	if (!OffEnd (list) && list->last == list->current)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * Precondition: List has been created with NewList.
 *               The list must not be empty.
 *
 * Returns the first void * pointer in the list.
 */

void* GetFirst (ListHndl list)
{
	void* temp;

	assert (list != NULL);
	assert (!IsEmpty (list));

	temp = list->first->data;
	return (temp);
}

/*
 * Precondition: List has been created with NewList.
 *               The list must not be empty.
 *
 * Returns the last void * pointer in the list.
 */

void* GetLast (ListHndl list)
{
	void* temp;

	assert (list != NULL);
	assert (!IsEmpty (list));

	temp = list->last->data;
	return (temp);
}

/*
 * Precondition: List has been created with NewList.
 *               Current must not equal NULL.
 *
 * Returns the void * pointer of the current node.
 */

void* GetCurrent (ListHndl list)
{
	void* temp;

	assert (list != NULL);

	temp = list->current->data;
	return (temp);
}

/*
 * Precondition: List has been created with NewList.
 *
 * Postcondition: The list is empty.
 *                Current is equal to NULL.
 *
 * Removes all nodes from the list.
 */

void MakeEmpty (ListHndl list)
{
	Node currNode = list->first;
	Node freeNode;

	assert (list != NULL);

	while (currNode != NULL)
	{
		freeNode = currNode;
		currNode = currNode->next;
		free (freeNode->data);
		free (freeNode);
	}

	list->first = NULL;
	list->last = NULL;
	list->current = NULL;

	assert (IsEmpty (list));
	assert (OffEnd (list));
}

/*
 * Precondition: List has been created with NewList.
 *               The list must not be empty.
 * 
 * Postcondition: Current cannot equal NULL.
 *
 * Sets the current node to be the first node in the list.
 */

void MoveFirst (ListHndl list)
{
	assert (list != NULL);
	assert (!IsEmpty (list));

	list->current = list->first;

	assert (!OffEnd (list));
}

/*
 * Precondition: List has been created with NewList.
 *               The list must not be empty.
 *
 * Postcondition: Current cannot equal NULL.
 *
 * Sets the current node to be the last node in the list.
 */


void MoveLast (ListHndl list)
{
	assert (list != NULL);
	assert (!IsEmpty (list));

	list->current = list->last;

	assert (!OffEnd (list));
}

/*
 * Precondition: List has been created with NewList.
 *               Current cannot equal NULL.
 *
 * Postcondition: Current equals NULL if current was at first before.
 *
 * Sets the current node to be the previous node in the list.
 */

void MovePrev (ListHndl list)
{
	assert (list != NULL);
	assert (!OffEnd (list));

	if (list->current->prev != NULL)
	{
		list->current = list->current->prev;
	}
	else
	{
		list->current = NULL;
	}
}

/*
 * Precondition: List has been created with NewList.
 *               Current cannot equal NULL.
 *
 * Postcondition: Current equals NULL if current was at last before.
 *
 * Sets the current node to be the next node in the list.
 */

void MoveNext (ListHndl list)
{
	assert (list != NULL);
	assert (!OffEnd (list));

	if (list->current->next != NULL)
	{
		list->current = list->current->next;
	}
	else
	{
		list->current = NULL;
	}
}

/*
 * Preconditions: List has been created with NewList.
 *
 * Postconditions: The list cannot be empty.
 *
 * Inserts a new node with the given void * pointer at the front of 
 * the list.
 */

void InsertAtFront (ListHndl list, void* data)
{
	Node newNode;

	assert (list != NULL);

	newNode = NewNode (data);

	if (list->first == NULL)
	{
		list->first = newNode;
		list->last = newNode;
	}
	else
	{
		newNode->next = list->first;
		list->first->prev = newNode;
		list->first = newNode;
	}

	assert (!IsEmpty (list));
}

/*
 * Preconditions: List has been created with NewList.
 *
 * Postconditions: The list cannot be empty.
 *
 * Inserts a new node with the given void * pointer at the back of 
 * the list.
 */

void InsertAtBack (ListHndl list, void* data)
{
	Node newNode;

	assert (list != NULL);

	newNode = NewNode (data);

	if (list->last == NULL)
	{
		list->first = newNode;
		list->last = newNode;
	}
	else
	{
		newNode->prev = list->last;
		list->last->next = newNode;
		list->last = newNode;
	}

	assert (!IsEmpty (list));
}

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

void InsertBeforeCurrent (ListHndl list, void* data)
{
	Node newNode;

	assert (list != NULL);
	assert (list->current != NULL);

	if (OffEnd (list))
	{
		fprintf (stderr, "There is no current node\n");
		return;
	}

	newNode = NewNode (data);

	if (AtFirst (list))
	{
		newNode->next = list->current;
		list->current->prev = newNode;
		list->first = newNode;
	}
	else
	{
		newNode->prev = list->current->prev;
		newNode->next = list->current;
		list->current->prev->next = newNode;
		list->current->prev = newNode;
	}

	assert (!IsEmpty (list));
	assert (list->current != list->first);
}

/*
 * Preconditions: List has been created with NewList.
 *                The list cannot be empty.
 *
 * Removes and frees the node at the front of the list.
 */

void DeleteFirst (ListHndl list)
{
	Node tempNode;

	assert (list != NULL);
	assert (!IsEmpty (list));

	if (list->current == list->first)
	{
		list->current = NULL;
	}

	tempNode = list->first;
	list->first->next->prev = NULL;
	list->first = list->first->next;

	free (tempNode->data);
	free (tempNode);
}

/*
 * Preconditions: List has been created with NewList.
 *                The list cannot be empty.
 *
 * Removes and frees the node at the back of the list.
 */

void DeleteLast (ListHndl list)
{
	Node tempNode;

	assert (list != NULL);
	assert (!IsEmpty (list));

	if (list->current == list->last)
	{
		list->current = NULL;
	}

	tempNode = list->last;
	list->last->prev->next = NULL;
	list->last = list->last->prev;

	free (tempNode->data);
	free (tempNode);
}

/*
 * Preconditions: List has been created with NewList.
 *                The list cannot be empty.
 *                Current must not equal NULL.
 *
 * Postconditions: Current equals NULL.
 *
 * Removes and frees the current node of the list.
 */

void DeleteCurrent (ListHndl list)
{
	Node tempNode;

	assert (list != NULL);
	assert (!IsEmpty (list));
	assert (list->current != NULL);

	if (IsEmpty (list))
	{
		fprintf (stderr, "The list is empty\n");
		return;
	}

	if (OffEnd (list))
	{
		fprintf (stderr, "There is no current node\n");
		return;
	}

	tempNode = list->current;

	if (list->current == list->first)
	{
		list->first = list->current->next;
		if (list->current->next != NULL)
		{
			list->current->next->prev = NULL;
		}
	}
	else
	{
		list->current->prev->next = list->current->next;
	}

	if (list->current == list->last)
	{
		list->last = list->current->prev;
		if (list->current->prev != NULL)
		{
			list->current->prev->next = NULL;
		}
	}
	else
	{
		list->current->next->prev = list->current->prev;
	}

	list->current = NULL;
	assert (list->current == NULL);

	free (tempNode->data);
	free (tempNode);
}
