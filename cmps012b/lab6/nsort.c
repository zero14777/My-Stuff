// $Id: nsort.c,v 1.1 2011-04-29 19:46:42-07 - - $
// Peter Greer-Berezovsky - pbgreerb
//
//
// NAME
//   nsort
//
// DESCRIPTION
//   Sorts numbers given to the stdin and sorts them into a stack in
//   ascending order. Prints them out from lowest value to highest
//   value at the end of the program.
//
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
   double item;
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

//Puts the given double into a node and places it in ascending order 
//on the stack.

void insert(double value, list *list){
   node *prev = NULL;
   node *curr = list->head;

   while (curr != NULL){
      if(curr->item > value) break;
      prev = curr;
      curr = curr->link;
   }
   
   node *temp = malloc(sizeof(struct node));
   assert(temp != NULL);

   temp->item = value;
   temp->link = curr;
   if(prev == NULL){
      list->head = temp;
   }
   else{
      prev->link = temp;
   }
}

//Takes in doubles of input and either prints an error message for
//the input not being a number, or sends the double to be put onto the
//stack.

void input(list *list){
   char err_buffer [256];
   char err_buf_fmt[16];
   sprintf (err_buf_fmt, "%%%lds", sizeof err_buffer - 1);
   for(;;){
      double scanVal;
      int value = scanf("%lf", &scanVal);
      if(value == EOF) break;
      if(value == 1)
         insert(scanVal, list);
      else{
         printf("not a number.\n");
         value = scanf (err_buf_fmt, err_buffer);
      }
   }
}

//Prints the doubles in stack out in order. The nodes in the list are
//freed after being printed.

void printList(list *list, options *opts){
   node *looker = list->head;
   while(looker != NULL){
      if(opts->debug_format == false)
         printf("%24.15g\n", looker->item);
      else
         printf("%p\n", &looker->item);
      looker = looker->link;
   }

   looker = list->head;
   while(looker != NULL){
      node *old = looker;
      looker = looker->link;
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
