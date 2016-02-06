#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdio.h>
using namespace std;


#include "symboltable.h"
#include "astree.h"
#include "lyutils.h"

symbol_table typeTable;

vector<symbol_table*> theSymbolStack;
vector<int> theBlocStack;

int next_bloc = 1;
int indent = 0;

int getCurrentBloc(){
  return theBlocStack.back();
}

void myFunctionToDoThatThing (astree* tree) {
  typeTable.erase(tree->children.at(0)->lexinfo);  
}

symbol* intern_typeTable (astree* tree) {
  symbol* temp = (symbol*)malloc(sizeof(struct symbol));
  temp->fields = NULL;
  temp->parameters = NULL;
  temp->attributes = ATTR_struct;
  temp->blocknr = 0;
  temp->filenr = tree->filenr;
  temp->linenr = tree->linenr;
  temp->offset = tree->offset;
  temp->initialized = 0;
  typeTable.insert ({tree->children.at(0)->lexinfo, temp});
  return temp;
}

symbol* find_typeTable (const string* key) {
  auto found = typeTable.find(key);
  if (found == typeTable.end()){
    return NULL;
  } else {
    return found->second;
  }
}

void symbolStackInit(){
  symbol_table* fake_table = NULL;
  theSymbolStack.push_back(fake_table);
  theBlocStack.push_back(0);
}

void symbolStackPush(){
  symbol_table* fake_table = NULL;
  theSymbolStack.push_back(fake_table);
  theBlocStack.push_back(next_bloc);
  next_bloc++;
  indent++;
}

void symbolStackPop(){
  theSymbolStack.pop_back();
  theBlocStack.pop_back();
  indent--;
}

void symbolStackAddVardecl(astree* tree, FILE* sym_file) {
  if (theSymbolStack.back() == NULL) {
    symbol_table* newTable = new symbol_table;
    theSymbolStack.back() = newTable;
  }
  auto table_check = theSymbolStack.back()->find(tree->lexinfo);
  if (table_check != theSymbolStack.back()->end()) {
    fprintf (stderr, "oc: double initialization of variable %s\n",
             tree->lexinfo->c_str());
    return;
  }
  symbol* temp = (symbol*)malloc(sizeof(struct symbol));
  temp->type_id = tree->type;  
  temp->fields = NULL;
  temp->parameters = NULL;
  temp->attributes = tree->attributes;
  temp->blocknr = theBlocStack.back();
  tree->blocknr = theBlocStack.back();
  temp->filenr = tree->filenr;
  temp->linenr = tree->linenr;
  temp->offset = tree->offset;
  temp->initialized = 0;
  theSymbolStack.back()->insert ({tree->lexinfo, temp});
  for(int n = 0; n < indent; n++){
    fprintf(sym_file, "   ");
  }
  fprintf(sym_file, "%s (%zu.%zu.%zu) {%d} ",
          tree->lexinfo->c_str(),
          temp->filenr, temp->linenr,
          temp->offset, theBlocStack.back());
  print_attrs (sym_file, temp->attributes,
               temp->type_id, NULL);
  fprintf(sym_file, "\n");
}

void symbolStackAddPrototype(astree* tree, FILE* sym_file) {
  if (theSymbolStack.back() == NULL) {
    symbol_table* newTable = new symbol_table;
    theSymbolStack.back() = newTable;
  }
  auto table_check = theSymbolStack.back()->find
    (tree->children.at(0)->children.back()->lexinfo);
  if (table_check != theSymbolStack.back()->end()) {
    fprintf (stderr, "oc: double declaration of function %s\n",
             tree->children.at(0)->children.back()->lexinfo->c_str());
    return;
  }
  symbol* temp = (symbol*)malloc(sizeof(struct symbol));
  temp->fields = NULL;
  temp->attributes = tree->attributes;
  temp->attributes.set(ATTR_function);
  if (tree->children.at(0)->symbol == TOK_ARRAY)
    { 
      temp->attributes.set(ATTR_array);
      tree->children.at(0)->attributes.set(ATTR_array);
      tree->children.at(0)->children.back()->attributes.set(ATTR_array);
      if (tree->children.at(0)->children.at(0)->symbol == TOK_BOOL)
        {
          temp->attributes.set(ATTR_bool);
          tree->children.at(0)->children.at(0)->
            attributes.set(ATTR_bool);
          tree->children.at(0)->children.back()->
            attributes.set(ATTR_bool);
        }
      else if (tree->children.at(0)->children.at(0)->symbol == TOK_CHAR)
        {
          temp->attributes.set(ATTR_char);
          tree->children.at(0)->children.at(0)->
            attributes.set(ATTR_char);
          tree->children.at(0)->children.back()->
            attributes.set(ATTR_char);
        }
      else if (tree->children.at(0)->children.at(0)->symbol == TOK_INT)
        {
          temp->attributes.set(ATTR_int);
          tree->children.at(0)->children.at(0)->
            attributes.set(ATTR_int);
          tree->children.at(0)->children.back()->
            attributes.set(ATTR_int);
        }
      else if (tree->children.at(0)->children.at(0)->
               symbol == TOK_STRING)
        { 
          temp->attributes.set(ATTR_string);
          tree->children.at(0)->children.at(0)->
            attributes.set(ATTR_string);
          tree->children.at(0)->children.back()->
            attributes.set(ATTR_string);
        }
      else if (tree->children.at(0)->children.at(0)->
               symbol == TOK_TYPEID)
        {
          temp->attributes.set(ATTR_struct);
          tree->children.at(0)->children.at(0)->
            attributes.set(ATTR_struct);
          tree->children.at(0)->children.back()->
            attributes.set(ATTR_struct);
          tree->children.at(0)->children.back()->type = 
            (string *)tree->children.at(0)->children.at(0)->lexinfo;
          tree->children.at(0)->children.at(0)->type = 
            (string *)tree->children.at(0)->children.at(0)->lexinfo;
          temp->type_id = 
            (string *)tree->children.at(0)->children.back()->lexinfo;
        }
      else
        {
          fprintf(stderr, "oc: Invalid array definition");
          return;
        }
    }
  else if (tree->children.at(0)->symbol == TOK_BOOL)
  {
    temp->attributes.set(ATTR_bool);
    tree->children.at(0)->attributes.set(ATTR_bool);
    tree->children.at(0)->children.back()->attributes.set(ATTR_bool);
  }
  else if (tree->children.at(0)->symbol == TOK_CHAR)
  {
    temp->attributes.set(ATTR_char);
    tree->children.at(0)->attributes.set(ATTR_char);
    tree->children.at(0)->children.back()->attributes.set(ATTR_char);
  }
  else if (tree->children.at(0)->symbol == TOK_INT)
  {
    temp->attributes.set(ATTR_int);
    tree->children.at(0)->attributes.set(ATTR_int);
    tree->children.at(0)->children.back()->attributes.set(ATTR_int);
  }
  else if (tree->children.at(0)->symbol == TOK_STRING)
  { 
    temp->attributes.set(ATTR_string);
    tree->children.at(0)->attributes.set(ATTR_string);
    tree->children.at(0)->children.back()->attributes.set(ATTR_string);
  }
  else if (tree->children.at(0)->symbol == TOK_VOID)
  { 
    temp->attributes.set(ATTR_void);
    tree->children.at(0)->attributes.set(ATTR_void);
    tree->children.at(0)->children.back()->attributes.set(ATTR_void);
  }
  else if (tree->children.at(0)->symbol == TOK_TYPEID)
  {
    temp->attributes.set(ATTR_struct);
    tree->children.at(0)->attributes.set(ATTR_struct);
    tree->children.at(0)->children.back()->attributes.set(ATTR_struct);
    tree->children.at(0)->children.back()->type = 
      (string *)tree->children.at(0)->lexinfo;
    tree->children.at(0)->type = 
      (string *)tree->children.at(0)->lexinfo;
    temp->type_id = 
      (string *)tree->children.at(0)->children.back()->lexinfo;
  }
  temp->blocknr = theBlocStack.back();
  temp->filenr = tree->filenr;
  temp->linenr = tree->linenr;
  temp->offset = tree->offset;
  temp->initialized = 0;
  for(int n = 0; n < indent; n++){
    fprintf(sym_file, "   ");
  }
  fprintf(sym_file, "%s (%zu.%zu.%zu) {%d} ",
          tree->children.at(0)->children.back()->lexinfo->c_str(),
          temp->filenr, temp->linenr,
          temp->offset, theBlocStack.back());
    print_attrs (sym_file, temp->attributes,
                 temp->type_id, NULL);
    fprintf(sym_file, "\n");
  temp->parameters = new vector<symbol*>;
  for(u_int i = 0; i < tree->children.at(1)->children.size(); i++) {
    if (theSymbolStack.back() == NULL) {
      symbol_table* newTable2 = new symbol_table;
      theSymbolStack.back() = newTable2;
    }
    symbol* parameter_symbol = 
      (symbol*)malloc(sizeof(struct symbol));    
    astree* params = tree->children.at(1)->children.at(i);
    parameter_symbol->fields = NULL;
    parameter_symbol->parameters = NULL;
    parameter_symbol->attributes = params->attributes;
    parameter_symbol->attributes.set(ATTR_lval);
    parameter_symbol->attributes.set(ATTR_variable);
    parameter_symbol->attributes.set(ATTR_param);
    if (params->symbol == TOK_ARRAY)
      { 
        parameter_symbol->attributes.set(ATTR_array);
        params->attributes.set(ATTR_array);
        params->children.back()->attributes.set(ATTR_array);
        if (params->children.at(0)->symbol == TOK_BOOL)
          {
            parameter_symbol->attributes.set(ATTR_bool);
            params->children.at(0)->attributes.set(ATTR_bool);
            params->children.back()->attributes.set(ATTR_bool);
          }
        else if (params->children.at(0)->symbol == TOK_CHAR)
          {
            parameter_symbol->attributes.set(ATTR_char);
            params->children.at(0)->attributes.set(ATTR_char);
            params->children.back()->attributes.set(ATTR_char);
          }
        else if (params->children.at(0)->symbol == TOK_INT)
          {
            parameter_symbol->attributes.set(ATTR_int);
            params->children.at(0)->attributes.set(ATTR_int);
            params->children.back()->attributes.set(ATTR_int);
          }
        else if (params->children.at(0)->symbol == TOK_STRING)
          { 
            parameter_symbol->attributes.set(ATTR_string);
            params->children.at(0)->attributes.set(ATTR_string);
            params->children.back()->attributes.set(ATTR_string);
          }
        else if (params->children.at(0)->symbol == TOK_TYPEID)
          {
            parameter_symbol->attributes.set(ATTR_struct);
            params->children.at(0)->attributes.set(ATTR_struct);
            params->children.back()->attributes.set(ATTR_struct);
            params->children.back()->type =
              (string *)params->children.at(0)->lexinfo;
            params->children.at(0)->type =
              (string *)params->children.at(0)->lexinfo;
            parameter_symbol->type_id =
              (string *)params->children.at(0)->lexinfo;
          }
      }
    else if (params->symbol == TOK_BOOL)
      {
        parameter_symbol->attributes.set(ATTR_bool);
        params->attributes.set(ATTR_bool);
        params->children.back()->attributes.set(ATTR_bool);
      }
    else if (params->symbol == TOK_CHAR)
      {
        parameter_symbol->attributes.set(ATTR_char);
        params->attributes.set(ATTR_char);
        params->children.back()->attributes.set(ATTR_char);
      }
    else if (params->symbol == TOK_INT)
    {
      parameter_symbol->attributes.set(ATTR_int);
      params->attributes.set(ATTR_int);
      params->children.back()->attributes.set(ATTR_int);
    }
    else if (params->symbol == TOK_STRING)
      { 
        parameter_symbol->attributes.set(ATTR_string);
        params->attributes.set(ATTR_string);
        params->children.back()->attributes.set(ATTR_string);
      }
    else if (params->symbol == TOK_TYPEID)
      {
        parameter_symbol->attributes.set(ATTR_struct);
        params->attributes.set(ATTR_struct);
        params->children.back()->attributes.set(ATTR_struct);
        params->children.back()->type = (string *)params->lexinfo;
        params->type = (string *)params->lexinfo;
        parameter_symbol->type_id = (string *)params->lexinfo;
      }
    parameter_symbol->blocknr = next_bloc;
    params->blocknr = next_bloc;
    params->children.at(0)->blocknr = next_bloc;
    params->children.back()->blocknr = next_bloc;
    parameter_symbol->filenr = params->filenr;
    parameter_symbol->linenr = params->linenr;
    parameter_symbol->offset = params->offset;
    temp->parameters->push_back(parameter_symbol);
    for(int n = 0; n < (indent + 1); n++){
      fprintf(sym_file, "   ");
    }
    
    fprintf(sym_file, "%s (%zu.%zu.%zu) {%d} ",
            params->children.back()->lexinfo->c_str(),
            parameter_symbol->filenr, parameter_symbol->linenr,
            parameter_symbol->offset, next_bloc);
    print_attrs (sym_file, parameter_symbol->attributes,
                 parameter_symbol->type_id, NULL);
    fprintf(sym_file, "\n");
  }
  symbolStackPush();
  symbolStackPop();
  fprintf(sym_file, "\n");
  theSymbolStack.back()->insert 
    ({tree->children.at(0)->children.back()->lexinfo, temp}); 
}

int symbolStackAddFunction(astree* tree, FILE* sym_file) {
  if (theSymbolStack.back() == NULL) {
    symbol_table* newTable = new symbol_table;
    theSymbolStack.back() = newTable;
  }
  auto table_check = theSymbolStack.back()->find
    (tree->children.at(0)->children.back()->lexinfo);
  if (table_check != theSymbolStack.back()->end() && 
      !table_check->second->attributes.test(ATTR_function)) {
    fprintf (stderr, 
     "oc: %s is already declared as something other than a function\n", 
             tree->children.at(0)->children.back()->lexinfo->c_str());
    return 0;
  } else if (table_check != theSymbolStack.back()->end() && 
             table_check->second->initialized == 1) {
    fprintf (stderr, "oc: double initialization of function %s\n",
             tree->children.at(0)->children.back()->lexinfo->c_str());
    return 0;
  } else {
    theSymbolStack.back()->erase
      (tree->children.at(0)->children.back()->lexinfo);
  }
  symbol* temp = (symbol*)malloc(sizeof(struct symbol));
  temp->fields = NULL;
  temp->initialized = 1;
  temp->attributes = tree->attributes;
  temp->attributes.set(ATTR_function);
  if (tree->children.at(0)->symbol == TOK_ARRAY)
  {
    temp->attributes.set(ATTR_array);
    tree->children.at(0)->attributes.set(ATTR_array);
    tree->children.at(0)->children.back()->attributes.set(ATTR_array);
    if (tree->children.at(0)->children.at(0)->symbol == TOK_BOOL)
      {
        temp->attributes.set(ATTR_bool);
        tree->children.at(0)->children.at(0)->attributes.set(ATTR_bool);
        tree->children.at(0)->children.back()->
          attributes.set(ATTR_bool);
      }
    else if (tree->children.at(0)->children.at(0)->symbol == TOK_CHAR)
      {
        temp->attributes.set(ATTR_char);
        tree->children.at(0)->children.at(0)->attributes.set(ATTR_char);
        tree->children.at(0)->children.back()->
          attributes.set(ATTR_char);
      }
    else if (tree->children.at(0)->children.at(0)->symbol == TOK_INT)
      {
        temp->attributes.set(ATTR_int);
        tree->children.at(0)->children.at(0)->attributes.set(ATTR_int);
        tree->children.at(0)->children.back()->attributes.set(ATTR_int);
      }
    else if (tree->children.at(0)->children.at(0)->
             symbol == TOK_STRING)
      { 
        temp->attributes.set(ATTR_string);
        tree->children.at(0)->children.at(0)->
          attributes.set(ATTR_string);
        tree->children.at(0)->children.back()->
          attributes.set(ATTR_string);
      }
    else if (tree->children.at(0)->children.at(0)->
             symbol == TOK_TYPEID)
      {
        temp->attributes.set(ATTR_struct);
        tree->children.at(0)->children.at(0)->
          attributes.set(ATTR_struct);
        tree->children.at(0)->children.back()->
          attributes.set(ATTR_struct);
        tree->children.at(0)->children.back()->type =
          (string *)tree->children.at(0)->children.at(0)->lexinfo;
        tree->children.at(0)->children.at(0)->type =
          (string *)tree->children.at(0)->children.at(0)->lexinfo;
        temp->type_id =
          (string *)tree->children.at(0)->children.back()->lexinfo;
      }
    else
      {
        fprintf(stderr, "oc: Invalid array definition");
        return 0;
      }
  }
  else if (tree->children.at(0)->symbol == TOK_BOOL)
  {
    temp->attributes.set(ATTR_bool);
    tree->children.at(0)->attributes.set(ATTR_bool);
    tree->children.at(0)->children.back()->attributes.set(ATTR_bool);
  }
  else if (tree->children.at(0)->symbol == TOK_CHAR)
  {
    temp->attributes.set(ATTR_char);
    tree->children.at(0)->attributes.set(ATTR_char);
    tree->children.at(0)->children.back()->attributes.set(ATTR_char);
  }
  else if (tree->children.at(0)->symbol == TOK_INT)
  {
    temp->attributes.set(ATTR_int);
    tree->children.at(0)->attributes.set(ATTR_int);
    tree->children.at(0)->children.back()->attributes.set(ATTR_int);
  }
  else if (tree->children.at(0)->symbol == TOK_STRING)
  { 
    temp->attributes.set(ATTR_string);
    tree->children.at(0)->attributes.set(ATTR_string);
    tree->children.at(0)->children.back()->attributes.set(ATTR_string);
  }
  else if (tree->children.at(0)->symbol == TOK_VOID)
  { 
    temp->attributes.set(ATTR_void);
    tree->children.at(0)->attributes.set(ATTR_void);
    tree->children.at(0)->children.back()->attributes.set(ATTR_void);
  }
  else if (tree->children.at(0)->symbol == TOK_TYPEID)
  {
    temp->attributes.set(ATTR_struct);
    tree->children.at(0)->attributes.set(ATTR_struct);
    tree->children.at(0)->children.back()->attributes.set(ATTR_struct);
    tree->children.at(0)->children.back()->type = 
      (string *)tree->children.at(0)->lexinfo;
    tree->children.at(0)->type = 
      (string *)tree->children.at(0)->lexinfo;
    temp->type_id = 
      (string *)tree->children.at(0)->children.back()->lexinfo;
  }
  temp->blocknr = theBlocStack.back();
  temp->filenr = tree->filenr;
  temp->linenr = tree->linenr;
  temp->offset = tree->offset;
  for(int n = 0; n < indent; n++){
    fprintf(sym_file, "   ");
  }
  fprintf(sym_file, "%s (%zu.%zu.%zu) {%d} ",
          tree->children.at(0)->children.back()->lexinfo->c_str(),
          temp->filenr, temp->linenr,
          temp->offset, theBlocStack.back());
    print_attrs (sym_file, temp->attributes,
                 temp->type_id, NULL);
    fprintf(sym_file, "\n");

  temp->parameters = new vector<symbol*>;
  for(u_int i = 0; i < tree->children.at(1)->children.size(); i++) {
    if (theSymbolStack.back() == NULL) {
      symbol_table* newTable2 = new symbol_table;
      theSymbolStack.back() = newTable2;
    }
    symbol* parameter_symbol = (symbol*)malloc(sizeof(struct symbol));
    astree* params = tree->children.at(1)->children.at(i);
    
    parameter_symbol->attributes = params->attributes;
    parameter_symbol->attributes.set(ATTR_lval);
    parameter_symbol->attributes.set(ATTR_variable);
    parameter_symbol->attributes.set(ATTR_param);
    if (params->symbol == TOK_ARRAY)
      { 
        parameter_symbol->attributes.set(ATTR_array);
        params->attributes.set(ATTR_array);
        params->children.back()->attributes.set(ATTR_array);
        if (params->children.at(0)->symbol == TOK_BOOL)
          {
            parameter_symbol->attributes.set(ATTR_bool);
            params->children.at(0)->attributes.set(ATTR_bool);
            params->children.back()->attributes.set(ATTR_bool);
          }
        else if (params->children.at(0)->symbol == TOK_CHAR)
          {
            parameter_symbol->attributes.set(ATTR_char);
            params->children.at(0)->attributes.set(ATTR_char);
            params->children.back()->attributes.set(ATTR_char);
          }
        else if (params->children.at(0)->symbol == TOK_INT)
          {
            parameter_symbol->attributes.set(ATTR_int);
            params->children.at(0)->attributes.set(ATTR_int);
            params->children.back()->attributes.set(ATTR_int);
          }
        else if (params->children.at(0)->symbol == TOK_STRING)
          { 
            parameter_symbol->attributes.set(ATTR_string);
            params->children.at(0)->attributes.set(ATTR_string);
            params->children.back()->attributes.set(ATTR_string);
          }
        else if (params->children.at(0)->symbol == TOK_TYPEID)
          {
            parameter_symbol->attributes.set(ATTR_struct);
            params->children.at(0)->attributes.set(ATTR_struct);
            params->children.back()->attributes.set(ATTR_struct);
            params->children.back()->type =
              (string *)params->children.at(0)->lexinfo;
            params->children.at(0)->type =
              (string *)params->children.at(0)->lexinfo;
            parameter_symbol->type_id =
              (string *)params->children.at(0)->lexinfo;
          }
      }
    else if (params->symbol == TOK_BOOL)
      {
        parameter_symbol->attributes.set(ATTR_bool);
        params->attributes.set(ATTR_bool);
        params->children.back()->attributes.set(ATTR_bool);
      }
    else if (params->symbol == TOK_CHAR)
      {
        parameter_symbol->attributes.set(ATTR_char);
        params->attributes.set(ATTR_char);
        params->children.back()->attributes.set(ATTR_char);
      }
    else if (params->symbol == TOK_INT)
    {
      parameter_symbol->attributes.set(ATTR_int);
      params->attributes.set(ATTR_int);
      params->children.back()->attributes.set(ATTR_int);
    }
    else if (params->symbol == TOK_STRING)
      { 
        parameter_symbol->attributes.set(ATTR_string);
        params->attributes.set(ATTR_string);
        params->children.back()->attributes.set(ATTR_string);
      }
    else if (params->symbol == TOK_TYPEID)
      {
        parameter_symbol->attributes.set(ATTR_struct);
        params->attributes.set(ATTR_struct);
        params->children.back()->attributes.set(ATTR_struct);
        params->children.back()->type = (string *)params->lexinfo;
        params->type = (string *)params->lexinfo;
        parameter_symbol->type_id = (string *)params->lexinfo;
      }
    parameter_symbol->fields = NULL;
    parameter_symbol->parameters = NULL;
    parameter_symbol->blocknr = next_bloc;
    params->blocknr = next_bloc;
    params->children.at(0)->blocknr = next_bloc;
    params->children.back()->blocknr = next_bloc;
    parameter_symbol->filenr = params->filenr;
    parameter_symbol->linenr = params->linenr;
    parameter_symbol->offset = params->offset;
    temp->parameters->push_back(parameter_symbol);
  }
  theSymbolStack.back()->insert
    ({tree->children.at(0)->children.back()->lexinfo, temp});
  tree->children.at(2)->blocknr = getCurrentBloc();
  symbolStackPush();
  for(u_int i = 0; i < tree->children.at(1)->children.size(); i++) {
    if (theSymbolStack.back() == NULL) {
      symbol_table* newTable2 = new symbol_table;
      theSymbolStack.back() = newTable2;
    }
    symbol* parameter_symbol = (symbol*)malloc(sizeof(struct symbol));
    astree* params = tree->children.at(1)->children.at(i);
    parameter_symbol->fields = NULL;
    parameter_symbol->parameters = NULL;
    parameter_symbol->attributes = params->attributes;
    parameter_symbol->attributes.set(ATTR_lval);
    parameter_symbol->attributes.set(ATTR_variable);
    parameter_symbol->attributes.set(ATTR_param);
    if (params->symbol == TOK_ARRAY)
      { 
        parameter_symbol->attributes.set(ATTR_array);
        if (params->children.at(0)->symbol == TOK_BOOL)
          {
            parameter_symbol->attributes.set(ATTR_bool);
          }
        else if (params->children.at(0)->symbol == TOK_CHAR)
          {
            parameter_symbol->attributes.set(ATTR_char);
          }
        else if (params->children.at(0)->symbol == TOK_INT)
          {
            parameter_symbol->attributes.set(ATTR_int);
          }
        else if (params->children.at(0)->symbol == TOK_STRING)
          { 
            parameter_symbol->attributes.set(ATTR_string);
          }
        else if (params->children.at(0)->symbol == TOK_TYPEID)
          {
            parameter_symbol->attributes.set(ATTR_struct);
            parameter_symbol->type_id =
              (string *)params->children.at(0)->lexinfo;
          }
      }
    else if (params->symbol == TOK_BOOL)
      {
        parameter_symbol->attributes.set(ATTR_bool);
      }
    else if (params->symbol == TOK_CHAR)
      {
        parameter_symbol->attributes.set(ATTR_char);
      }
    else if (params->symbol == TOK_INT)
      {
        parameter_symbol->attributes.set(ATTR_int);
      }
    else if (params->symbol == TOK_STRING)
      { 
        parameter_symbol->attributes.set(ATTR_string);
      }
    else if (params->symbol == TOK_VOID)
      { 
        parameter_symbol->attributes.set(ATTR_void);
      }
    else if (params->symbol == TOK_TYPEID)
      {
        parameter_symbol->attributes.set(ATTR_struct);
        parameter_symbol->type_id = (string *)params->lexinfo;
      }
    parameter_symbol->blocknr = next_bloc;
    parameter_symbol->filenr = params->filenr;
    parameter_symbol->linenr = params->linenr;
    parameter_symbol->offset = params->offset;
    theSymbolStack.back()->insert
      ({params->children.back()->lexinfo, parameter_symbol});
    for(int n = 0; n < indent; n++){
      fprintf(sym_file, "   ");
    }
    
    fprintf(sym_file, "%s (%zu.%zu.%zu) {%d} ",
            params->children.back()->lexinfo->c_str(),
            parameter_symbol->filenr, parameter_symbol->linenr,
            parameter_symbol->offset, theBlocStack.back());
    print_attrs (sym_file, parameter_symbol->attributes,
                 parameter_symbol->type_id, NULL);
    fprintf(sym_file, "\n");
  }
  fprintf(sym_file, "\n");
  return 1;
}

symbol* getInfo (const string* ident)
{
  int i = (theSymbolStack.size() - 1);
        for(; i >= 0; i--)
        {
          if(theSymbolStack.at(i) == NULL) {
            continue;
          }
          auto table_check = theSymbolStack.at(i)->find(ident);
          if(table_check != theSymbolStack.at(i)->end())
            {
              return table_check->second;
            }
        }
        fprintf(stderr, "oc: no %s found\n", ident->c_str());
        return NULL;
}

symbol* grabFunctionSymbol (astree* tree)
{
  if(theSymbolStack.at(0) == NULL) {
    return NULL;
  }
  auto table_check = theSymbolStack.at(0)->find(tree->lexinfo);
  if(table_check != theSymbolStack.at(0)->end())
    {
      return table_check->second;
    }
  fprintf(stderr, "oc: %s function does not exist\n",
          tree->lexinfo->c_str());
  return NULL;
}

symbol* grabStructSymbol (const string* ident)
{
  auto table_check = typeTable.find(ident);
  if(table_check != typeTable.end())
    {
      return table_check->second;
    }
  fprintf(stderr, "oc: %s struct does not exist\n", ident->c_str());
  return NULL;
}
