
#include <vector>
#include <string>
using namespace std;

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "lyutils.h"
#include "auxlib.h"
#include "astree.h"

astree* yyparse_astree = NULL;
int scan_linenr = 1;
int scan_offset = 0;
bool scan_echo = false;
vector<string> included_filenames;

const std::string* lexer_filename (int filenr) {
   return &included_filenames.at(filenr);
}

void lexer_newfilename (const char* filename) {
   included_filenames.push_back (filename);
}

void lexer_newline (void) {
   ++scan_linenr;
   scan_offset = 0;
}

void lexer_setecho (bool echoflag) {
   scan_echo = echoflag;
}

void lexer_useraction (void) {
   if (scan_echo) {
      if (scan_offset == 0) printf (";%5d: ", scan_linenr);
      printf ("%s", yytext);
   }
   scan_offset += yyleng;
}

void yyerror (const char* message) {
   badsyntax = 1;
   assert (not included_filenames.empty());
   errprintf ("%:%s: %d: %s\n",
              included_filenames.back().c_str(),
              scan_linenr, message);
}

void lexer_badchar (unsigned char bad) {
   char char_rep[16];
   badtok = 1;
   sprintf (char_rep, isgraph (bad) ? "%c" : "\\%03o", bad);
   errprintf ("%:%s: %d: invalid source character (%s)\n",
              included_filenames.back().c_str(),
              scan_linenr, char_rep);
}

void lexer_badtoken (char* lexeme) {
   badtok = 1;
   errprintf ("%:%s: %d: invalid token (%s)\n",
              included_filenames.back().c_str(),
              scan_linenr, lexeme);
}

int yylval_token (int symbol) {
   int offset = scan_offset - yyleng;
   yylval = new astree 
     (0, 0, nullptr, symbol, included_filenames.size() - 1,
      scan_linenr, offset, 0, 0, 0, 0, yytext, nullptr);
   fprintf(tok_file, "%4d%4d.%03d %4d %-16s (%s)\n",
           (int)yylval->filenr,
           (int)yylval->linenr,
           (int)yylval->offset,
           yylval->symbol,
           get_yytname(yylval->symbol),
           yylval->lexinfo->c_str());
   //   free_ast (yylval);
   return symbol;
}

astree* new_parseroot (void) {
  yyparse_astree = new astree 
    (0, 0, nullptr, TOK_ROOT, 0, 0, 0, 0, 0, 0, 0, "", nullptr);
   return yyparse_astree;
}

astree* new_parsefunction (astree* root) {
  return new astree 
    (0, 0, nullptr, TOK_FUNCTION, root->filenr, root->linenr,
     root->offset, 0, 0, 0, 0, "", nullptr);
}

astree* new_parseprototype (astree* root) {
  return new astree 
    (0, 0, nullptr, TOK_PROTOTYPE, root->filenr, root->linenr,
     root->offset, 0, 0, 0, 0, "", nullptr);
}

void lexer_include (FILE *file) {
   lexer_newline();
   char filename[strlen (yytext) + 1];
   int linenr;
   int scan_rc = sscanf (yytext, "# %d \"%[^\"]\"",
                         &linenr, filename);
   if (scan_rc != 2) {
     errprintf ("%: %d: [%s]: invalid directive, ignored\n",
                scan_rc, yytext);
   }else {
     fprintf (file, "# %d \"%s\"\n", linenr, filename);
     lexer_newfilename (filename);
     scan_linenr = linenr - 1;
     DEBUGF ('m', "filename=%s, scan_linenr=%d\n",
             included_filenames.back().c_str(), scan_linenr);
   }
}

/*
static void* yycalloc (size_t size) {
   void* result = calloc (1, size);
   assert (result != NULL);
   return result;
}
*/

RCSC("$Id: lyutils.cpp,v 1.5 2015-04-09 19:34:17-07 - - $")

