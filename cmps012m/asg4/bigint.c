// $Id: bigint.c,v 1.12 2013-05-16 15:14:31-07 - - $
// Peter Greer-Berezovsky pbgreerb
// James Brower jrbrwoer
//
// NAME
//   bigint
//
// SYNOPSIS
//   Contains functions pertaining to the bigint struct. Functions are
//   set up to perform addition, subtraction, and multiplication on
//   two bigints. Contains a print that will print the character array
//   of a bigint which will appear as the number that the bigint 
//   represents.

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

static void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}

bigint *new_string_bigint (char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   DEBUGS ('b', show_bigint (this));
   return this;
}

// Accepts two bigints and creates a new bigint called result. Sets 
// results size to be 1 greater than the larger of the two sizes of 
// the given bigints. Uses an algorithm to add each of the char arrays
// given and store their sum in results char array. Negativity is 
// determined after do_add is called to lessen the amount of code. 
// Trims zeros and returns result.

static bigint *do_add (bigint *this, bigint *that) {
   size_t resultcap = 0;

   if (this->size > that->size) {
      resultcap = this->size + 1;
   }
   else {
      resultcap = that->size + 1;
   }

   bigint *result = new_bigint(resultcap);
   assert(NULL != result);
   result->size = resultcap;

   unsigned int i = 0;
   char carry = 0;
   while (i < resultcap) {
      char thisD = 0;
      char thatD = 0;
      
      if (i < this->size) {
         thisD = this->digits[i];
      }
      if (i < that->size) {
         thatD = that->digits[i];
      }

      char number = thisD + thatD + carry;
      result->digits[i] = number % 10;
      carry = number / 10;
      ++i;
   }
   
   trim_zeros(result);
   return result;
}

// Accepts two bigints and creates a result. Sets results size to be 
// equal to the size of the given bigint with the largest size. Checks
// which of the bigints is greater and determines negativity based on 
// which is greater. Uses an algorithm to subtract the smaller char 
// array from the larger char array and store their result in results
// char array. Trims zeros and returns result.

static bigint *do_sub (bigint *this, bigint *that) {
   size_t newCap;
   newCap = (this->size > that->size ? this->size : that->size);
   bigint *result = new_bigint(newCap);
   int i = 0;
   int borrow = 0;
   char *larger;
   char *smaller;
   int lSize;
   int sSize;
   if(this->size > that->size){
      larger = this->digits; 
      lSize = this->size;
      smaller = that->digits;
      sSize = that->size;
      result->negative = false;
   } 
   else if(this->size < that->size){
      larger = that->digits;
      lSize = that->size;
      smaller = this->digits;
      sSize = this->size;
      result->negative = true;
   } 
   else {
      int sizeI = this->size - 1;
      while(sizeI > 0){
         if((this->digits[sizeI]) > (that->digits[sizeI])){
            larger = this->digits;
            lSize = this->size;
            smaller = that->digits;
            sSize = that->size;
            result->negative = false;
            break;
         } else if((that->digits[sizeI]) > (this->digits[sizeI])){
            larger = that->digits;
            lSize = that->size;
            smaller = this->digits;
            sSize = this->size;
            result->negative = true; 
            break;
         }
         sizeI++;   
      }
      if(this->size == 1 ){
         int resultD = this->digits[0] - that->digits[0];
         if(resultD < 0){
            resultD = (resultD * -1);
            result->negative = true;
         }
         result->digits[0] = resultD;
         lSize = 1;
         i = 1;
      }
   }

   while(i < lSize){
      int largerD = (i < lSize ? (larger[i]) : 0);
      int smallerD = (i < sSize ? (smaller[i]) : 0);
      int digit = largerD - smallerD - borrow + 10;
      result->digits[i] = (digit%10);
      borrow = 1 - (digit / 10);
      i++;
   }

   result->size = newCap;
   trim_zeros(result);
   return result;
}

// Frees the memory allocated to the given bigint.

void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

// Prints the char array of the given bigint in order to the given
// file. If the given bigint's negative boolean is true a '-' is 
// printed before the string is.

void print_bigint (bigint *this, FILE *file) {
   if (this->negative) {
      fprintf(file, "%c", '-');
   }
   for(int i = this->size - 1; i >= 0; --i) {
      fprintf(file, "%c", this->digits[i] + '0');
   }
   if(this->size == 0){
      fprintf(file, "0");
   }
   fprintf(file, "\n");
}

// Creates a pointer to a bigint called result. Calls the do_add or
// do_sub functions depending on the negative booleans of the given
// bigints. If do_add is called negativity is determined in this
// function. Result is set to the returned value from do_add or
// do_sub. result is returned.

bigint *add_bigint (bigint *this, bigint *that) {
   bigint *result;
   if((this->negative && that->negative) ||
      (!this->negative && !that->negative)) {
      result = do_add(this, that);
      if(this->negative && that->negative) {
         result->negative = true;
      } 
      else {
         result->negative = false;
      }
      return result;
   } 
   else if(this->negative) {
      result = do_sub(that, this);
      return result;
   } 
   else {
      result = do_sub(this, that);
      return result;
   }
}

// Creates a pointer to a bigint called result. Calls the do_add or 
// do_sub functions depending on the negative booleans of the given 
// bigints. If do_add is called negativity is determined in this 
// function. Result is set to the returned value from do_add or 
// do_sub. result is returned.

bigint *sub_bigint (bigint *this, bigint *that) {
   bigint *result;
   if((!this->negative && !that->negative) ||
      (this->negative && that->negative)){
      result = do_sub(this, that);
      return result;
   } else if(this->negative){
      result = do_add(this, that);
      result->negative = true;
      return result;
   } else {
      result = do_add(this, that);
      result->negative = false;
      return result;
   }
}

// Accepts two bigints and creates a pointer to a bigint called 
// result. Sets the size of result to be the sum of the sizes of the
// two given bigints. Determines which of the bigints has a greater
// size. Uses an algorithm to multiply each of the char arrays given
// and store their product in results char array. Determines 
// negativity based on the negativity of the given bigints. Trims 
// zeros and returns result.

bigint *mul_bigint (bigint *this, bigint *that) {
   bigint *result = new_bigint(this->size + that->size);
   char *smaller;
   char *larger;
   int small;
   int large;
   if(this->size < that->size){
      smaller = this->digits;
      larger = that->digits;
      small = this->size;
      large = that->size;
   } else {
      smaller = that->digits;
      larger = this->digits;
      small = that->size;
      large = this->size;
   }
   
   int carry = 0;
   int smallI = 0;
   while(smallI < small){
      int largeI = 0;
      while(largeI < large || carry != 0){
         int digit;
         if(largeI < large){
            digit = result->digits[largeI+smallI] + 
               (smaller[smallI] * larger[largeI]) + carry;
         } else {
            digit = carry;
         }
         result->digits[largeI+smallI] = digit % 10;
         carry = digit / 10;
         largeI++;
      }
      smallI++;
   }

   if((this->negative && that->negative) ||
      (!this->negative && !that->negative)){
      result->negative = false;
   } else {
      result->negative = true;
   }
  
   result->size = (this->size + that->size);
   trim_zeros(result);
   return result;
}

void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}

