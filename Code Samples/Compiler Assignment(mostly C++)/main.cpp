// $Id: main.cpp,v 1.3 2015-03-25 19:03:01-07 - - $
// Peter Greer-Berezovsky   pbgreerb@ucsc.edu
// Jim Brower               jrbrower@ucsc.edu

// This is the main file for a simple compiler a partner and I
// worked on together. . Due to time constraints during the quarter we
// ended up having to stop early and never implemented the final
// step of code generation.
// 

#include <string>
using namespace std;

#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lyutils.h"
#include "auxlib.h"
#include "stringset.h"
#include "astree.h"
#include "trevorsal.h"

FILE *tok_file;
FILE *ast_file;
FILE *sym_file = NULL;
int badtok;
int badsyntax;
const string CPP = "/opt/rh/devtoolset-2/root/usr/bin/cpp";

int main (int argc, char **argv) {
  int failed = 0;
  badsyntax = 0;
  badtok = 0;
  set_execname(argv[0]);
  string cpp_arg = "";
  char* noext = NULL;
  yy_flex_debug = yydebug = 0;

  for(;;){
    int option = getopt (argc, argv, "ly@:D:");
    if (option == EOF) break;
    switch (option) {
    case 'l': 
      yy_flex_debug = 1;
      break;
    case 'y': 
      yydebug = 1;
      break;
    case '@': 
      {
        set_debugflags (optarg);
        break;
      }
    case 'D': 
      {
        string Darg = optarg;
        cpp_arg = "-D " + Darg;
        break;
      }
    default : break;
    }
  }
  
  if (optind >= argc) 
    {
      fprintf (stderr, "oc: No Input file given\n");
      return EXIT_FAILURE;
    }
  
  char* filename = argv[optind];
  char* bname = basename(filename);
  char* extension = strstr(bname, ".");
  
  if (NULL != extension) {
    size_t difference = extension - bname;
    if (0 == difference)
      {
        fprintf(stderr, "oc: Invalid filename\n");
        return EXIT_FAILURE;
      }
    noext = (char *)malloc((difference  + 1) * sizeof(*noext));
    noext = strncpy(noext, bname, difference);
    bname = noext;
  }
  
  string command = CPP + " " + cpp_arg + " " + filename;
  
  // Make pipe to the file to be compiled
  
  yyin = popen (command.c_str(), "r");
  
  // If the pipe failed to open fail out
  
  if (yyin == NULL) {
    fprintf (stderr, "oc: Failed to open pipe to cpp\n");
    if (NULL != extension) {
      free(noext);
    }
    return EXIT_FAILURE;
  }
  
  // Here we prepare output files to display the results of
  // lexical analysis done by the parser,
  
  else
    {
      string tokFilename = bname;
      string astFilename = bname;
      string symFilename = bname;
      tokFilename = tokFilename + ".tok";
      astFilename = astFilename + ".ast";
      symFilename = symFilename + ".sym";
      tok_file = fopen(tokFilename.c_str(), "w");
      ast_file = fopen(astFilename.c_str(), "w");
      sym_file = fopen(symFilename.c_str(), "w");
	  
	  /* This function uses some c++ code produced by the yacc
	     file parser.y to tokenize the code from the input file
	     and store it in an abstract syntax tree.
	  */
      yyparse();
	  
	  /* This function traverses the tree created by the parser
		 and checks for syntax errors. It also checks for scope
		 errors using a symbol table.
	  */
      dfsTraversal (yyparse_astree);
	  
	  // This function prints the tree to a file.
      dump_astree(ast_file, yyparse_astree);
      fclose(sym_file);
      fclose(tok_file);
      fclose(ast_file);
    }
  
  int pipeCheck = pclose(yyin);
  string strFilename = bname;
  
  if(NULL != extension) {
    free(noext);
  }
  
  // This code dumps info on the every string in the input file.
  strFilename = strFilename + ".str";
  FILE* file = fopen(strFilename.c_str(), "w");
  dump_stringset (file);
  fclose(file); 

  free_ast(yyparse_astree);

  if (pipeCheck != 0) {
    fprintf(stderr, "oc: Some error occurred in cpp\n");
    failed = 1;
  }
  if (badtok == 1) {
    fprintf(stderr, "oc: Some error occurred in the scanner\n");
    failed = 1;
  }
  if (badsyntax == 1) {
    fprintf(stderr, "oc: Syntax error found in parser\n");
    failed = 1;
  }

  if (failed == 1 ) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
