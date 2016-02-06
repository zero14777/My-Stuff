#ifndef __INSSORT_H__
#define __INSSORT_H__

#include <stdlib.h>
#include <string.h>

void inssort (void *base, size_t nelem, size_t size, 
              int (*compar) (const void *, const void *));

#endif
