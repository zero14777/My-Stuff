//Peter Greer-Berezovsky pbgreerb
//
//This program takes chars as input from stdin, and puts them into a 
//buffer. At the end of each line the buffer is put into the array. At
//the end of stdin main  will call the inssort function to sort the 
//char *'s into ascending lexiographic order. The char *'s are then 
//printed out in ascending lexiographic order.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "inssort.h"

//prints the array from its first to its last elemnt

void printarray (char **array, size_t nelem) {
   for (size_t itor = 0; itor < nelem; ++itor) {
      printf ("%s\n", array[itor]);
   }
}

//Uses strcmp to compare two char arrays and returns an int. If the
//first is lexiographically greater than the second the int returned
//is positive. If the second is lexiographically greater than the first
//the int returned is negative. 0 is returned if they are both equal.

int lineCmp (const void *this, const void *that) {
   char **thisline = (char **) this;
   char **thatline = (char **) that;
   return (strcmp(*thisline, *thatline));
}

//Takes input from stdin and calls the sort and print functions.
//Allocated data is freed before ending.

int main (void) {
   char *array[1000];
   memset(array, 0, (1000 * sizeof(char *)));
   char buffer[1024];
   int i;
   for (i = 0; i < 1000; ++i) {
      char *line = fgets(buffer, sizeof(buffer), stdin);

      if (!line) break;

      char *newline = strchr(buffer, '\n');
      if (NULL != newline) {
         *newline = '\0';
         array[i] = strdup(buffer);
      }
   }


   inssort (array, i, sizeof(char *), lineCmp);
   printarray (array, i);
   for(; i >= 0; i--){
      free(array[i]);
   }
   return EXIT_SUCCESS;
}
