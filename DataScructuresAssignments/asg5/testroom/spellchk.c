// $Id: spellchk.c,v 1.6 2013-05-21 19:58:24-07 - - $
//
// NAME
//    spellchk
//
// DESCRIPTION
//    Reads lines in from a default dictionary file and puts them into
//    a hashset. Also reads another dictionary if the user inputs the
//    -d [filename] option. The default dictionary is not used if the
//    user inputs the -n option. Prints out information about the 
//    hashset if the -x option is used. Also prints out the whole
//    hashset if the -x option is used twice. -y, -@, and -@@ turn on
//    the scanners, DEBUGF's, and all debug flags respectively. When a
//    line is typed to stdin all words in that line are checked with 
//    the hashset to see if they were in the given directories(case 
//    insensitive). Any word that is not in the hashset gets printed
//    out indicating it was misspelled. All allocated memory is freed
//    and the program closes when stdin ends.
//    


#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "debugf.h"
#include "hashset.h"
#include "yyextern.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

//Prints out the proper error message.

void print_error (char *object, char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", Exec_Name, object, message);
   fflush (NULL);
   Exit_Status = EXIT_FAILURE;
}

// Opens the given file in read form and returns the opened file. 
// Prints out an error if the file is NULL.

FILE *open_infile (char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) print_error (filename, strerror (errno));
   return file;
}

// Takes input from filename's location, then checks to see if each
// word is contained in the hashset. If the word is not contained a
// case insensitive check is preformed. If both checks failed a line
// is printed to stdout that gives the filename the word is from, the
// words line number, and the word that is misspelled. Breaks out of
// its loop and returns when "filename" reaches eof.

void spellcheck (char *filename, hashset *hashset) {
   yylineno = 1;
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      bool isWord = has_hashset(hashset, yytext);
      if (isWord == false){
         char *yytextlow = strdup(yytext);
         for(int i = 0; yytextlow[i]; ++i) {
            yytextlow[i] = tolower(yytextlow[i]);
         }
         isWord = has_hashset(hashset, yytextlow);
         free(yytextlow);
      }
      if (isWord == false){
         printf("%s: %d: %s\n", filename, yylineno, yytext);
      }
   }
}

// Checks if a file should be accepted, trys to open a file, then 
// cheks to see if that file exists. Reads lines from input and puts
// them into the hashset. Closes the opened dictionary file.

void load_dictionary (char *dictionary_name, hashset *hashset) {
   if (dictionary_name == NULL) return;
   FILE *dictionary = open_infile(dictionary_name);
   if (dictionary == NULL) return;
   for(;;){
      char buffer[1024];
      char *linepos = fgets(buffer, 1023, dictionary);
      if (linepos == NULL){
         break;
      }
      int i = 0;
      for(; buffer[i] != '\n'; i++){}
      buffer[i] = '\0';
      put_hashset(hashset, buffer);
   }
   fclose(dictionary);
}

// Checks for options and calls functions or sets values accordingly.
// Loads the hashset with words from the designated dictionaries. 
// Calls spellchk to check words in given files or stdin.

int main (int argc, char **argv) {
   Exec_Name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   hashset *hashset = new_hashset ();
   yy_flex_debug = false;
   bool x = false;
   bool xx = false;

   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': user_dictionary = optarg;
                   break;
         case 'n': default_dictionary = NULL;
                   break;
         case 'x': if(x) xx = true;
                   x = true;
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debugflags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);

   if(hashsetload(hashset) == 0){
      fprintf(stderr, "%s: dictionary is empty\n", argv[0]);
      free_hashset(hashset);
      Exit_Status = EXIT_FAILURE;
      return Exit_Status;
   }

   if(x) hashprint(hashset);
   if(xx) {
      hashprintmore(hashset);
      free_hashset(hashset);
      return Exit_Status;
   }

   // Read and do spell checking on each of the files.
   if (optind >= argc) {
      yyin = stdin;
      spellcheck (STDIN_NAME, hashset);
   }else {
      for (int fileix = optind; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }

   yylex_destroy ();
   free_hashset(hashset);
   return Exit_Status;
}

