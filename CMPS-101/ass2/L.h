/*
 * Peter Greer-Berezovsky
 * pbgreerb
 * 4/3/2014
 *
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * This .h file corresponds to the List.c file.
 */

typedef struct NodeStruct* Node;

typedef struct ListStruct* ListHndl;

ListHndl NewList ();

Node NewNode (long data);

void FreeList (ListHndl list);

int IsEmpty (ListHndl list);

int OffEnd (ListHndl list);

int AtFirst (ListHndl list);

int AtLast (ListHndl list);

long GetFirst (ListHndl list);

long GetLast (ListHndl list);

long GetCurrent (ListHndl list);

void MakeEmpty (ListHndl list);

void MoveFirst (ListHndl list);

void MoveLast (ListHndl list);

void MovePrev (ListHndl list);

void MoveNext (ListHndl list);

void InsertAtFront (ListHndl list, long data);

void InsertAtBack (ListHndl list, long data);

void InsertBeforeCurrent (ListHndl list, long data);

void InsertionSort (ListHndl list, long data);

void DeleteFirst (ListHndl list);

void DeleteLast (ListHndl list);

void DeleteCurrent (ListHndl list);

void PrintList (FILE* out, ListHndl list);

void PrintCurrent (FILE* out, ListHndl list);
