// Peter Greer-Berezovsky   pbgreerb@ucsc.edu
// Jim Brower               jrbrower@ucsc.edu

#ifndef __SYMBOLSET__
#define __SYMBOLSET__

#include <string>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <stdio.h>

#include "attr_bitset.h"

using namespace std;

/*
enum { ATTR_void, ATTR_bool, ATTR_char, ATTR_int, ATTR_null,
       ATTR_string, ATTR_struct, ATTR_array, ATTR_function,
       ATTR_variable, ATTR_field, ATTR_typeid, ATTR_param,
       ATTR_lval, ATTR_const, ATTR_vreg, ATTR_vaddr,
       ATTR_bitset_size,
};
*/

struct astree;
struct symbol;

using attr_bitset = bitset<ATTR_bitset_size>;
using symbol_table = unordered_map<const string*, symbol*>;
using symbol_entry = symbol_table::value_type;


struct symbol {
   attr_bitset attributes;
   symbol_table* fields;
   size_t filenr, linenr, offset;
   size_t blocknr;
   string* type_id;
   vector<symbol*>* parameters;
   int initialized;
};

int getCurrentBloc();

void myFunctionToDoThatThing (astree* tree);

symbol* intern_typeTable (astree* tree);

symbol* find_typeTable (const string* key);

void symbolStackInit ();

void symbolStackPush ();

void symbolStackPop ();

void symbolStackAddVardecl (astree* tree, FILE* sym_file);

void symbolStackAddPrototype (astree* tree, FILE* sym_file);

int symbolStackAddFunction (astree* tree, FILE* sym_file);

symbol* getInfo(const string* ident);

symbol* grabFunctionSymbol (astree* tree);

symbol* grabStructSymbol (const string* ident);
#endif
