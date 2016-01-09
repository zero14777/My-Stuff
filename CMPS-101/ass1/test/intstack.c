/* 
 *     Source code (.c) file for stack-of-integers ADT
 *
 * This contains the implementation of the operations and types 
 * exported by the intstack.h file.
 */

#include "myinclude.h"     /* Some C defines I routinely use */
#include "intstack.h"      /* The ADT header file */
#include <assert.h>        /* This allows me to use the "assert" macro */

/* 
 * The file intstack.h contains the following:
 *
 * typedef struct IntStackStruct * StackHndl;
 * void NewStack (StackHndl *Stack);
 * void FreeStack (StackHndl *Stack);
 * Boolean IsEmpty (StackHndl Stack);
 * int Top (StackHndl Stack);
 * void Pop (StackHndl *Stack);
 * void Push (StackHndl *Stack);
 */

/* 
 * Additional types used throughout the ADT implementation:
 * StackFrame: a pointer to a struc containing a 
 *             StackFrame pointer and an int.
 */

typedef struct StackFrameStruct * StackFrame;


typedef struct IntStackStruct 
{
  StackFrame StackTop;    /* This is the end of the sequence */
  int        Size;        /* the number of int's in the  sequence */
} IntStackStruct;

typedef struct StackFrameStruct
{
  int        Data;        /* The int stored in the frame */
  StackFrame Prev;        /* The frame for the previous int in the sequence */
} StackFrameStruct;

/* 
 * GLOBAL VARIABLES:
 * The main global variables used in this implementation are the
 * deal with the free list.  This is a list of free stack frames
 * which can be used to accomodate Push operations.  
 */

StackFrame FreeList;
int NumStacks = 0;  /* This keeps track of the number of active stacks */

/*
 * UTILITY FUNCTION PROTOTYPES
 * These functions are "helpers" to help implement the exported operations.
 */


StackFrame NewStackFrame();  /* gets a new frame from the free list or malloc */

void FreeFrame (StackFrame Frame); /* puts un-needed frames on free list */

void FreeFreeList (); /* Free the storage used by the free list */

void PrintStack (StackHndl Stack); /* prints the sequence in reverse */


/*
 * EXPORTED FUNCTIONS
 */

/*
 * The procedure "NewStack" creates a new stack object
 * and returns a pointer to it.  This function should be used
 * to initialize each stack before any other operations are performed
 * on that stack.  There is no precondition.
 * The postcondition is that the stack is initialized to an empty stack.
 */

StackHndl NewStack ()
{
  StackHndl tempStack;
  tempStack = malloc ( sizeof(IntStackStruct) );
  tempStack->Size = 0;
  tempStack->StackTop = NULL;
  /* printf("NewStack: numstacks was %d \n", NumStacks); */
  NumStacks = NumStacks + 1;
  return (tempStack);
}  


/* 
 * The procedure "FreeStack" should be called once on each
 * stack when the stack is no longer. Its purpose is to free up 
 * the storage used by the stack.  
 * Precondition: Stack has been initialized by "NewStack",
 *               Stack need not be empty.
 * Postcondition: Stack is set to NULL
 * Note: "Boolean" is defined in myinclude.h
 */

void FreeStack (StackHndl *Stack)
{
  assert ((*Stack) != NULL); 

  while (! IsEmpty((*Stack))) { Pop (*Stack); }
  free ( (*Stack) );
  (*Stack) = NULL;
  /* printf("FreeStack: NumStacks was %d \n", NumStacks); */
  NumStacks = NumStacks - 1;

  if (NumStacks == 0) { FreeFreeList (); }
}
      

/* The access function IsEmpty returns true if the sequence
 * represented by the stack is the empty sequence.
 * Precondition: none
 * Postcondition: returns true if the sequence is empty,
 *                and false otherwise.
 */

Boolean IsEmpty (StackHndl Stack)
{
  assert (Stack != NULL);
  return (Stack->Size == 0);
}
    

/*
 * The access function Top returns the integer at the top of the stack.
 * Precondition: not IsEmpty
 * Postcondition: returns the last integer in the sequence.
 */

int Top (StackHndl Stack)
{
  assert (Stack != NULL);
  assert (! IsEmpty(Stack));
  return (Stack->StackTop->Data);
}

/* 
 * The manipulation procedure Pop changes the state (sequence of 
 * integers represented by) the stack. 
 * Precondition: stack not empty 
 * Postcondition: sequence (a, b, ..., x, y, z) is changed to the
 *         sequence (a, b, ..., x, y).
 */

void Pop (StackHndl Stack)
{
  StackFrame  TempFrame; 

  assert (Stack != NULL);
  assert (! IsEmpty(Stack));
  
  TempFrame = Stack->StackTop;   /* Save frame to put on free list */
  Stack->StackTop = Stack->StackTop->Prev;
  Stack->Size = Stack->Size - 1;
  FreeFrame (TempFrame);
}  

/* 
 * The manipulation procedure Push adds an element to the sequence.
 * Precondition: none
 * Postcondition: when z is pushed, the sequence (a, b, ..., y) is
 *                changed to the sequence (a, b, ..., y, z)
 */

void Push (StackHndl Stack, int DataVal)
{
  StackFrame  TempFrame; 

  assert (Stack != NULL);

  TempFrame = NewStackFrame();
  TempFrame->Data = DataVal;
  TempFrame->Prev = Stack->StackTop;
  Stack->StackTop = TempFrame;
  Stack->Size = Stack->Size + 1;
}
  
/* IMPLEMENTATIONS OF THE UTILITY FUNCTIONS */

/* gets a new frame from the free list or malloc */
StackFrame NewStackFrame()
{
  StackFrame TempFrame;
  
  if (FreeList == NULL)
    {
      TempFrame = malloc ( sizeof( StackFrameStruct ));
    }
  else
    {
      TempFrame = FreeList;
      FreeList = FreeList->Prev;
    }

  TempFrame->Data = -1357;
  TempFrame->Prev = NULL;
  return(TempFrame);
}

/* puts un-needed frames on free list */
void FreeFrame (StackFrame Frame)
{
  Frame->Data = -7531;
  Frame->Prev = FreeList;
  FreeList = Frame;
}

/* Free the storage used by the free list */ 
void FreeFreeList ()
{
  StackFrame TempFrame;

  /*  printf("Freeing Free list \n"); */

  while (FreeList != NULL)
    {
      TempFrame = FreeList;
      FreeList = FreeList->Prev;
      free (TempFrame);
    }
}

void PrintStack (StackHndl Stack) /* prints the sequence in reverse */
{
  StackFrame TempFrame;

  assert (! IsEmpty (Stack));

  TempFrame = Stack->StackTop;
  printf("Printing Stack in reverse order (%d  elements) <", Stack->Size);
  
  while (TempFrame != NULL)
    {
      printf("%d ", TempFrame->Data);
      TempFrame = TempFrame->Prev;
    }

  printf("> \n");
}



