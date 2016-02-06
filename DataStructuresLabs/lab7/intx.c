// $Id: intx.c,v 1.8 2013-05-01 13:22:21-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intx.h"

struct intx {
   int value;
};

intx *new_intx (void) {
   return new_int_intx (0);
}

intx *new_int_intx (int value) {
   intx *this = malloc (sizeof (struct intx));
   assert (this != NULL);
   this->value = value;
   return this;
}

void free_intx (intx *this) {
   free (this);
}

int get_intx (intx *this) {
   return this->value;
}

void put_intx (intx *this, int newvalue) {
   this->value = newvalue;
}

// 
// Notes that would normally not be put in the file:
// 
// A '.c' file always includes its own header.
// 
// The 'struct' definition itself is specified in the
// implementation file.  Everything declared in the implementation
// file is 'private'.  Never put field definitions in a header
// file.
// 

