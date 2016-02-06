// $Id: numsort.c,v 1.2 2013-12-04 23:25:27-08 - - $
//Peter Greer-Berezovsky pbgreerb
//
//This program takes input from stdin in the form of doubles,
//and puts them into an array. It will then call the inssort
//function to sort the doubles given. The then sorted doubles
//are printed out in ascending order, one double per line.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "inssort.h"

//Prints the double array from first to last element.

void printdoubles (double *array, size_t nelem) {
   for (size_t itor = 0; itor < nelem; ++itor) {
      printf ("%20.15g\n", array[itor]);
   }
}

//Compares two doubles, a positive int is returned if the first 
//double is greater. A negative int is returned if the second 
//double is greater. 0 is returned if they are both equal.

int doubleCmp (const void *this, const void *that) {
   double *thisdouble = (double*) this;
   double *thatdouble = (double*) that;
   return (*thisdouble > *thatdouble) - (*thisdouble < *thatdouble);
}

//Takes in doubles from stdin and puts them into an array. If stdin 
//reaches end of file, over 1000 doubles are given or a non-double 
//input is given input stops being accepted. In any of these cases 
//inssort is called and the double array is sorted. printdoubles is
//then called and the doubles are printed in ascending order. 

int main (void) {
   double array[1000];
   memset(array, 0, sizeof(double));

   size_t i = 0;
   for (;i < 1000; i++) {
      double input = 0;
      int eofTest = scanf("%lf", &input);
      if(eofTest != 1) break;
      array[i] = input;
}

   inssort (array, i, sizeof(double), doubleCmp);
   printdoubles (array, i);

   return EXIT_SUCCESS;
}
