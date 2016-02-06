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

void printarray (char **array, size_t nelem) {
   for (size_t itor = 0; itor < nelem; ++itor) {
      printf ("%s\n", array[itor]);
   }
}

int lineCmp (const void *this, const void *that) {
   char **thisline = (char **) this;
   char **thatline = (char **) that;
   return (strcmp(*thisline, *thatline));
}

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
