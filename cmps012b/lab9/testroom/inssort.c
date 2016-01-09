// Peter Greer-Berezovsky pbgreerb
//
// A sorting function that will accept an array of generic elements,
// the size of that array, the size of the element, and a function 
// to compare two elements. This function will sort the elements using
// the given compare function into ascending order.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "inssort.h"

void inssort (void *array, size_t nelem, size_t size,
             int (*compare) (const void *, const void *)) {
   char *charArray = (char *) array;
   char *copy = malloc(size);
   for (size_t sorted = 1; sorted < nelem; ++sorted) {
      char *slot = charArray + (sorted * size);
      copy = memcpy(copy, slot, size);
      for (; slot > charArray; slot = slot - size) {
         char *slotprev = slot - size;
         int cmp = compare (copy, slotprev);
         if (cmp > 0)  break;
         slot = memcpy (slot, slotprev, size);
      }
      slot = memcpy (slot, copy, size);
   }
   free(copy);
}
