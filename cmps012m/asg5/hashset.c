// $Id: hashset.c,v 1.7 2013-05-23 15:41:07-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugf.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %d, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

// Returns the value of a hashsets load for use outside of hashset.c

size_t hashsetload(hashset *this){
   return this->load;
}

// Frees all the parts of the hashset.

void free_hashset (hashset *this) {
   for(size_t i = 0; i < this->size; i++){
      hashnode *curr = this->chains[i];
      while(curr){
         hashnode *temp = curr;
         curr = curr->link;
         free(temp->word);
         free(temp);
      }
   }
   free(this->chains);
   free(this);
}

// Prints out information about the size, number of words, chains, and
// clusters of a given hashset.

void hashprint(hashset *this){
   int words = 0;
   size_t maxchaincheck = 1000;
   size_t maxclustercheck = 1000;
   int chainlen[maxchaincheck];
   memset(chainlen, 0, sizeof(chainlen));
   int clusters[maxclustercheck];
   memset(clusters, 0, sizeof(clusters));
   int clustersize = 0;

   for(size_t i = 0; i < this->size; i++){
      if(this->chains[i]){
         clustersize++;
      } else {
         clusters[clustersize]++;
         clustersize = 0;
      }

      int depth = 0;
      hashnode *curr = this->chains[i];
      while(curr){
         depth++;
         words++;
         curr = curr->link;
      }
      chainlen[depth]++;
   }
   clusters[clustersize]++;

   printf("%5d words in the hash set\n", words);
   printf("%5lu size of the hash array\n", (unsigned long)this->size);
   for(size_t i = 1; i < maxchaincheck; i++){
      if(chainlen[i] != 0){
         printf("%5d chains of size %3lu\n", chainlen[i], 
            (unsigned long)i);
      }
   }
   for(size_t i = 2; i < maxclustercheck; i++){
      if(clusters[i] != 0){
         printf("%5d clusters of size %3lu\n",clusters[i], 
            (unsigned long)i);
      }
   }
}

// Prints out every word stored in a hashset along with its hashvalue
// and position in the hashset's array.

void hashprintmore(hashset *this){
   for(size_t i = 0; i < this->size; i++){
      hashnode *curr = this->chains[i];
      if(curr){
         printf("array[%10lu] = %20lu \"%s\"\n", i, 
            (unsigned long)strhash(curr->word), curr->word);
         curr = curr->link;
      }
      for(; curr; curr = curr->link){
         printf("                  = %20lu \"%s\"\n",
            (unsigned long)strhash(curr->word), curr->word);
      }
   }
}

// Reallocates the chains of the hashset to be 1 greater than double
// what they were previously. Each node is replaced into the new chains
// and the old chains are freed.

void reallocatehash(hashset *this){
   size_t prevsize = this->size;
   this->size = 1 + (2 * this->size);
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   hashnode **newchains = malloc (sizeof_chains);
   assert (newchains != NULL);
   memset (newchains, 0, sizeof_chains);
   for(size_t i = 0; i < prevsize; i++){
      hashnode *curr = this->chains[i];
      while(curr){
         hashnode *tmp = curr->link;
         size_t hashval = (strhash(curr->word) % this->size);
         curr->link = newchains[hashval];
         newchains[hashval] = curr;
         curr = tmp;
      }
   }
   free(this->chains);
   this->chains = newchains;
}

// Gets a hashset and word. Calls reallocatehash if the given 
// hashset's load is twice it's current size. Iterates through 
// the chains of the hashset to see if the given word is already 
// there. If the word is not in the hashset a new hashnode is 
// allocated and the word is put into the hashset.

void put_hashset (hashset *this, char *item) {
   if((this->load * 2) > this->size){
      reallocatehash(this);
   }
   size_t hash_value = (strhash(item) % this->size);
   hashnode *curr = this->chains[hash_value];
   bool isThere = false;
   while(curr){
      if(strcmp(curr->word, item) == 0){
         isThere = true;
         break;
      }
      curr = curr->link;
   }
   if(!isThere){
      hashnode *newnode = malloc(sizeof(*newnode));
      assert(newnode != NULL);
      newnode->link = this->chains[hash_value];
      newnode->word = strdup(item);
      this->chains[hash_value] = newnode;
      this->load++;
   }
}

// Gets a hashset and word. Iterates through the chains of the hashset
// to see if it contains the given word. If the hashset contains the
// word true is returned, if not false is returned.

bool has_hashset (hashset *this, char *item) {
   size_t hash_value = (strhash(item)) % this->size;
   hashnode *curr = this->chains[hash_value];
   while(curr){
      if(strcmp(curr->word, item) == 0){
         return true;
      }
      curr = curr->link;
   }
   return false;
}
