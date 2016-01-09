/* 
 * Driver program for the IntStack module.
 * The purpose of this is not to do anything "interesting"
 * but rather to test out and make sure that the intstack
 * ADT implementation is working properly.
 */

#include "myinclude.h"

#include "intstack.h"

int main()
{
  StackHndl FirstStack;
  StackHndl SecStack;

  FirstStack = NULL;
  SecStack = NULL;

  FirstStack = NewStack ();
  SecStack = NewStack ();

/* 
 * The next call has been commented out as 
 * it triggers the "assert" mechanism.  Put it
 * back in to see how the asserts in intstack.c work.
 */
/*  Top (SecStack); */

  Push (FirstStack, 11);
  Push (FirstStack, 22);
  Push (FirstStack, 33);

  if (IsEmpty(FirstStack) ) {printf ("IsEmpty test failed \n");}
  else {printf("IsEmpty test OK \n"); }

/*
 * Note the use of "self-diagnosing output.  With just a little
 * thought you can make the output even more self-diagnosing.
 */

  printf("Should be 33, is: %d \n", Top(FirstStack));
  Pop (FirstStack);

  printf("Should be 22, is: %d \n", Top(FirstStack));
  Pop (FirstStack);

  printf("Should be 11, is: %d \n", Top(FirstStack));

  if (IsEmpty(FirstStack) ) {printf ("IsEmpty test failed \n");}
  else {printf("IsEmpty test OK \n"); }

  Pop (FirstStack);

  if (IsEmpty(FirstStack) ) {printf("IsEmpty test OK \n"); }
  else {printf ("IsEmpty test failed \n");}

  if (IsEmpty(SecStack) ) {printf("IsEmpty test OK \n"); }
  else {printf ("IsEmpty test failed \n");}

  Push(SecStack, 111);
  Push(SecStack, 222);
  Push(SecStack, 333);
  Push(SecStack, 444);
  
/* 
 * This call requires that the PrintStack routine
 * prototype is in the .h file (or else it can't be
 * called by the client).  I really good driver
 * might be printing the FreeList as well, and have
 * a PrintFreeList procedure in the .h file.  These
 * Print procedures are only in the .h file for testing,
 * and would be commented out in the final .h file.
 */

  PrintStack(SecStack);

  FreeStack(&FirstStack);
  FreeStack(&SecStack);
  return(0);
}
