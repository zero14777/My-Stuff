// $Id: nsort.c,v 1.1 2011-04-29 19:46:42-07 - - $
// Peter Greer-Berezovsky - pbgreerb
//
//
// NAME
//   lsort
//
// DESCRIPTION
//   Sorts strings given to the stdin and sorts them into a stack in
//   ascending order. Prints them out from lowest value to highest
//   value at the end of the program.
//
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct options options;
struct options {
    bool debug_format;
};

typedef struct node node;
struct node{
   char *item;
   node *link;
};

typedef struct list list;
struct list{
   node *head;
};

//Sets the options values based on the options given. Will print an
//error message if invalid options are given.

void scan_options (int argc, char **argv, options *opts) {
    opts->debug_format = false;
    opterr = false;
    for (;;) {
        int opt = getopt (argc, argv, "d");
        if (opt == EOF) break;
        switch (opt) {
           case 'd':
               opts->debug_format = true;
               break;
           default:
               fflush (NULL);
               fprintf (stderr, "%s: -%c: invalid option\n",
                   argv[0], optopt);
           break;
      }
   }
}

//Puts the given string into a node and places it in ascending order 
//on the stack.

void insert(char *line, list *list){
   node *prev = NULL;
   node *curr = list->head;

   while (curr != NULL){
      if(strcmp(curr->item, line) > 0) break;
      prev = curr;
      curr = curr->link;
   }
   
   node *temp = malloc(sizeof(struct node));
   assert(temp != NULL);

   temp->item = strdup(line);
   temp->link = curr;
   if(prev == NULL){
      list->head = temp;
   }
   else{
      prev->link = temp;
   }
}

//Takes in strings of input and either prints an error message for 
//the line being too long, or sends the string to be put onto the
//stack.

void input(list *list){
   char buffer [82];

   for(;;){
      char *line = fgets(buffer, 82, stdin);
      if(line == NULL) break;
      
      char *nLPos = strchr (buffer, '\n');
      
      if(nLPos != NULL){
         *nLPos = '\0';
         insert(buffer, list);
      }
      else {
         fprintf(stderr, "lsort: Line too long.");
      }
   }
}

//Prints the strings in stack out in order. The strings are freed
//after being printed.

void printList(list *list, options *opts){
   node *looker = list->head;
   while(looker != NULL){
      if(opts->debug_format == false)
         printf("%s\n", looker->item);
      else
         printf("%p\n", &looker->item);
      looker = looker->link;
   }

   looker = list->head;
   while(looker != NULL){
      node *old = looker;
      looker = looker->link;
      free (old->item);
      free (old);
   }
}

//creates the list and opts structures that are used throughout the 
//program, calls other functions, and frees the list and opts just
//before ending.

int main (int argc, char **argv) {
   options *opts = malloc(sizeof(struct options));
   scan_options (argc, argv, opts);
   
   list *list = malloc(sizeof(struct list));
   list->head = NULL;
   
   input(list);
   printList(list, opts);
   free(opts);
   free(list);
}
