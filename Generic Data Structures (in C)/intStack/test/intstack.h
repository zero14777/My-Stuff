/* 
 *     Header (.h) file for stack-of-integers ADT
 *
 * This contains the operations and types exported
 * by the module.  The module implements a the ADT "stack"
 * using dynamic memory allocation.  The state of a stack
 * is a (possibly empty) sequence of integers, with the
 * "top" of the stack at the end (left).
 *
 * USE OF THIS ADT ALSO REQUIRES THAT "myinclude.h" be
 * included.
 */


/* 
 * The following defines the stack handle type.
 * Users should declare (and initialize to null)
 * one variable of type "StackHndl" for each stack they
 * want to have.  After declaration (and initialization
 * to null), the "NewStack" procedure must be called before
 * any stack operations can be performed.
 */

typedef struct IntStackStruct * StackHndl;


/*
 * The procedure "NewStack" creates a new stack object
 * and returns a pointer to it.  This function should be used
 * to initialize each stack before any other operations are performed
 * on that stack.  There is no precondition.
 * The postcondition is that the stack is initialized to an empty stack.
 */

StackHndl NewStack ();


/* 
 * The procedure "FreeStack" should be called once on each
 * stack when the stack is no longer. Its purpose is to free up 
 * the storage used by the stack.  
 * Precondition: Stack has been initialized by "NewStack",
 *               Stack need not be empty.
 * Postcondition: Stack is set to null
 * Note: "Boolean" is defined in myinclude.h
 */

void FreeStack (StackHndl *Stack);

/* The access function IsEmpty returns true if the sequence
 * represented by the stack is the empty sequence.
 * Precondition: none
 * Postcondition: returns true if the sequence is empty,
 *                and false otherwise.
 */

Boolean IsEmpty (StackHndl Stack);


/*
 * The access function Top returns the integer at the top of the stack.
 * Precondition: not IsEmpty
 * Postcondition: returns the last integer in the sequence.
 */

int Top (StackHndl Stack);

/* 
 * The manipulation procedure Pop changes the state (sequence of 
 * integers represented by) the stack. 
 * Precondition: stack not empty 
 * Postcondition: sequence (a, b, ..., x, y, z) is changed to the
 *         sequence (a, b, ..., x, y).
 */

void Pop (StackHndl Stack);

/* 
 * The manipulation procedure Push adds an element to the sequence.
 * Precondition: none
 * Postcondition: when z is pushed, the sequence (a, b, ..., y) is
 *                changed to the sequence (a, b, ..., y, z)
 */

void Push (StackHndl Stack, int DataVal);

/* 
 * PrintStack is a utility function that displays the contents of the stack
 */

void PrintStack(StackHndl Stack);