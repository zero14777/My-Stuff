// $Id: main.cpp,v 1.3 2015-03-25 19:03:01-07 - - $
// Peter Greer-Berezovsky   pbgreerb@ucsc.edu
// Jim Brower               jrbrower@ucsc.edu

#include <string>
using namespace std;

#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "auxlib.h"
#include "stringset.h"

const string CPP = "/opt/rh/devtoolset-2/root/usr/bin/cpp";
const size_t LINESIZE = 1024;

int main (int argc, char **argv) {

  string cpp_arg = "";
  char* noext = NULL;

  for(;;){
    int option = getopt (argc, argv, "ly@:D:");
    if (option == EOF) break;
    switch (option) {
      //char opttop_string[16]; // used for error printing in default
    case 'l': 
      // yy_flex_debug = 1;
      break;
    case 'y': 
      // yydebug = 1;
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
  
  // make pipe
  FILE* pipe = popen (command.c_str(), "r");
  if (pipe == NULL) {
    fprintf (stderr, "oc: Failed to open pipe to cpp\n");
    if (NULL != extension) {
      free(noext);
    }
    return EXIT_FAILURE;
  }
  else
    {
      for(;;) {
        char buffer[LINESIZE];
        char* fgets_return = fgets (buffer, LINESIZE, pipe);
        if (fgets_return == NULL){
          break;
        }
        //tokenize contents of pipe
        char* savepos = NULL;
        char* bufptr = buffer;
        for (int tokencount = 1;; ++tokencount) {
          char* token = strtok_r (bufptr, " \t\n", &savepos);
          bufptr = NULL;
          if (token == NULL) {
            break;
          }
          
          //put tokens in string_set
          intern_stringset(token);
        }
      }
    }
  
  int pipeCheck = pclose(pipe);
  string strFilename = bname;

  if (pipeCheck != 0) {
    fprintf(stderr, "oc: Some error occured in cpp\n");
    if (NULL != extension) {
      free(noext);
    }
    strFilename = strFilename + ".str";
    FILE* file = fopen(strFilename.c_str(), "w");
    dump_stringset (file);
    fclose(file); 
    return EXIT_FAILURE;
  }
  
  if(NULL != extension) {
    free(noext);
  }
  strFilename = strFilename + ".str";
  FILE* file = fopen(strFilename.c_str(), "w");
  dump_stringset (file);
  fclose(file); 
  return EXIT_SUCCESS;
}
