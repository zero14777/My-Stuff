#ifndef __ASTREE_H__
#define __ASTREE_H__

#include <string>
#include <vector>
#include <bitset>
using namespace std;

#include "auxlib.h"
#include "attr_bitset.h"

struct symbol;
using attr_bitset = bitset<ATTR_bitset_size>;

struct astree {
  string *type;
  attr_bitset attributes;
  int blocknr;
  struct symbol* struct_table_node;

  int symbol;               // token code
  size_t filenr;            // index into filename stack
  size_t linenr;            // line number from source code
  size_t offset;            // offset of token with current line
  size_t decfilenr;         // declaration index into filename
  size_t declinenr;         // declaration line number from source
  size_t decoffset;         // declaration offset of token 
  int printdecs;
  const string* lexinfo;    // pointer to lexical information
  vector<astree*> children; // children of this n-way node
  astree (attr_bitset attributes, int blocknr,
          struct symbol* struct_table_node,
          int symbol, int filenr, int linenr,
          int offset, int decfilenr, int declinenr,
          int decoffset, int printdecs, 
          const char* clexinfo, string* type);
};

// Append one child to the vector of children.
astree* adopt1 (astree* root, astree* child);

// Append two children to the vector of children.
astree* adopt2 (astree* root, astree* left, astree* right);

// Append three children to the vector of children.
astree* adopt3 (astree* root, astree* first,
                astree* second, astree* third);

// Append one child to the vector of
//children and give the root a symbol.
astree* adopt1sym (astree* root, astree* child, int symbol);

// Change the symbol of an astree node.
astree* changeSym (astree* root, int symbol);

// Dump an astree to a FILE.
void dump_astree (FILE* outfile, astree* root);

// Debug print an astree.
void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep);

// Recursively free an astree.
void free_ast (astree* tree);

// Recursively free two astrees.
void free_ast2 (astree* tree1, astree* tree2);

void print_attrs (FILE *out, attr_bitset attributes,
                  string *type, string *fieldof);

RCSH("$Id: astree.h,v 1.4 2015-04-09 19:31:47-07 - - $")
#endif
