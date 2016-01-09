// $Id: crpn.c,v 1.13 2013-10-19 17:23:55-07 - - $

//
// NAME
//    crpn - a Reverse Polish notation calculator
//
// SYNOPSIS
//    1 1 + ;
//    2.000000
//
// DESCRIPTION
//    This program will keep track of values given to is and preform
//    operators given to it in Reverse Polish notation.
//

#include <assert.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

int exit_status = EXIT_SUCCESS;
#define EMPTY (-1)
#define SIZE 16

typedef struct stack stack;
struct stack {
   int top;
   double numbers[SIZE];
};

//bad_operator prints out an error message if improper input is given

void bad_operator (const char *oper) {
   fflush (NULL);
   fprintf (stderr, "\"%s\": invalid operator\n", oper);
   fflush (NULL);
   exit_status = EXIT_FAILURE;
}

//push will check for stack overflow then push the given value to the
//top of the stack if there is space.

void push (stack *the_stack, double number) {
   if (the_stack->top >= SIZE - 1) {
         printf ("%f: stack overflow\n", number);
      }else {
         the_stack->numbers[++the_stack->top] = number;
      }
}

//do_binop will take the top two values on the stack and preform
//the designated operation with them. The new value will then
//be pushed onto the stack.

void do_binop (stack *the_stack, char oper) {
   if (the_stack->top < 1) {
         printf ("'%d': stack underflow\n", oper);
      }else {
         double right = the_stack->numbers[the_stack->top--];
         double left = the_stack->numbers[the_stack->top--];
         switch (oper) {
            case '+': push (the_stack, left + right); break;
            case '-': push (the_stack, left - right); break;
            case '*': push (the_stack, left * right); break;
            case '/': push (the_stack, left / right); break;
         }
      }
}

//do_print will print outthe stack in order from bottom to top or
//indicate that the stack is empty.

void do_print (stack *the_stack) {
   if (the_stack->top == EMPTY) {
         printf ("stack is empty\n");
      }else {
         int pos = 0;
         for (;pos <= the_stack->top; ++pos) {
            printf ("%f\n", the_stack->numbers[pos]);
         }
      }
}

//do_clear will clear the entire stack so that it contains no values

void do_clear (stack *the_stack) {
   the_stack->top = EMPTY;
}

//do_operator will call the function for the passed operator or will
//call the bad operator function.

void do_operator (stack *the_stack, const char *oper) {
   switch (oper[0]) {
         case '+': do_binop (the_stack, '+'); break;
         case '-': do_binop (the_stack, '-'); break;
         case '*': do_binop (the_stack, '*'); break;
         case '/': do_binop (the_stack, '/'); break;
         case ';': do_print (the_stack);      break;
         case '@': do_clear (the_stack);      break;
         default : bad_operator (oper);   break;
      }
}

//main creates the stack and takes input. main starts a loop that
//will not end until there is an error or the program is forced to 
//stop.

int main (int argc, char **argv) {
   if (argc != 1) {
      fprintf (stderr, "Usage: %s\n", basename (argv[0]));
      fflush (NULL);
      exit (EXIT_FAILURE);
   }
   stack the_stack;
   the_stack.top = EMPTY;
   char buffer[1024];
   for (;;) {
      int scanrc = scanf ("%1023s", buffer);
      if (scanrc == EOF) break;
      assert (scanrc == 1);
      if (buffer[0] == '#') {
         scanrc = scanf ("%1023[^\n]", buffer);
         continue;
      }
      char *endptr;
      double number = strtod (buffer, &endptr);
      if (*endptr == '\0') {
         push (&the_stack, number);
      }else if (buffer[1] != '\0') {
         bad_operator (buffer);
      }else {
         do_operator (&the_stack, buffer);
      }
   }
   return exit_status;
}

