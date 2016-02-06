#include <stdio.h>
#include <string.h>
using namespace std;

#include "trevorsal.h"
#include "astree.h"
#include "lyutils.h"
#include "symboltable.h"

extern FILE *sym_file;

attr_bitset types = 0;

void setAttributes (attr_bitset dest, attr_bitset src)
{
  if (src.test(ATTR_array))
    { 
      dest.set(ATTR_array);
    }
  else if (src.test(ATTR_bool))
    { 
      dest.set(ATTR_bool);
    }
  else if (src.test(ATTR_char))
    {
      dest.set(ATTR_char);
    }
  else if (src.test(ATTR_int))
    {
      dest.set(ATTR_int);
    }
  else if (src.test(ATTR_string))
    { 
      dest.set(ATTR_string);
    }
  else if (src.test(ATTR_struct))
    {
      dest.set(ATTR_struct);
    }
}

void dfsTraversalStruct (astree* tree, symbol* struct_symbol,
                         string* struct_name) {

  symbol* temp_field = (symbol*)malloc(sizeof(struct symbol));
  if(struct_symbol->fields == NULL) {
    struct_symbol->fields = new symbol_table;
  }
  temp_field->fields = NULL;
  temp_field->parameters = NULL;
  if (tree->symbol == TOK_ARRAY)
    { 
      tree->attributes.set(ATTR_array);
      tree->children.back()->attributes.set(ATTR_array);

      if (tree->children.at(0)->symbol == TOK_BOOL)
        {
          tree->children.at(0)->attributes.set(ATTR_bool);
          tree->children.back()->attributes.set(ATTR_bool);
        }

      else if (tree->children.at(0)->symbol == TOK_CHAR)
        {
          tree->children.at(0)->attributes.set(ATTR_char);
          tree->children.back()->attributes.set(ATTR_char);
        }

      else if (tree->children.at(0)->symbol == TOK_INT)
        {
          tree->children.at(0)->attributes.set(ATTR_int);
          tree->children.back()->attributes.set(ATTR_int);
        }

      else if (tree->children.at(0)->symbol == TOK_STRING)
        { 
          tree->children.at(0)->attributes.set(ATTR_string);
          tree->children.back()->attributes.set(ATTR_string);
        }

      else if (tree->children.at(0)->symbol == TOK_TYPEID)
        {
          tree->children.at(0)->attributes.set(ATTR_struct);
          tree->children.back()->type =
            (string *)tree->children.at(0)->lexinfo;
          tree->children.back()->attributes.set(ATTR_struct);
        }
    }

  else if (tree->symbol == TOK_BOOL)
    {
      tree->attributes.set(ATTR_bool);
      tree->children.back()->attributes.set(ATTR_bool);
    }

  else if (tree->symbol == TOK_CHAR)
    {
      tree->attributes.set(ATTR_char);
      tree->children.back()->attributes.set(ATTR_char);
    }

  else if (tree->symbol == TOK_INT)
    {
      tree->attributes.set(ATTR_int);
      tree->children.back()->attributes.set(ATTR_int);
    }

  else if (tree->symbol == TOK_STRING)
    { 
      tree->attributes.set(ATTR_string);
      tree->children.back()->attributes.set(ATTR_string);
    }

  else if (tree->symbol == TOK_TYPEID)
    {
      tree->attributes.set(ATTR_struct);
      tree->children.back()->attributes.set(ATTR_struct);
      tree->children.back()->type = (string *)tree->lexinfo;      
    }

  tree->children.back()->attributes.set(ATTR_field);
  temp_field->attributes = tree->children.back()->attributes;
  temp_field->type_id = tree->children.back()->type;

  temp_field->blocknr = 0;
  temp_field->filenr = tree->filenr;
  temp_field->linenr = tree->linenr;
  temp_field->offset = tree->offset;
  struct_symbol->fields->insert ({tree->children.back()->lexinfo,
        temp_field});
  fprintf(sym_file, "   %s (%zu.%zu.%zu) "
 ,
          tree->children.back()->lexinfo->c_str(),
          temp_field->filenr, temp_field->linenr,
          temp_field->offset
          );
  print_attrs (sym_file, temp_field->attributes,
               temp_field->type_id, struct_name);
  fprintf(sym_file, "\n");
}

astree *first_typeid;
astree *second_typeid;

string *global_lexinfo = nullptr;
astree *declid;
astree *fun_declid;


bool compatible(attr_bitset first, attr_bitset second,
                string *first_typeid, string *second_typeid)
{
  bool ret = false;
  if ((first & types) == (second & types))
    {
      // they are structs so need to check typeid
      if (first[ATTR_struct] && second[ATTR_struct])
        {
          // same typeid
          if (first_typeid == second_typeid) ret = true;
        }
      else ret = true;
    }

  // ref type and null
  else if (   (first[ATTR_null]   &&   second[ATTR_null])
              || (first[ATTR_string] &&   second[ATTR_null])
              || (first[ATTR_struct] &&   second[ATTR_null])
              || (first[ATTR_array]  &&   second[ATTR_null])
              || (first[ATTR_null]   && second[ATTR_string])
              || (first[ATTR_null]   && second[ATTR_struct])
              || (first[ATTR_null]   &&  second[ATTR_array]))
    {
      ret = true;
    }

  return ret;
  // one is array and one isn't
  //  else if (first[ATTR_array] != second[ATTR_array]) return false;
}

bool any(attr_bitset attributes)
{
  // if its void then it isn't 'any'
  if (attributes[ATTR_void]) return false;

  // if its not void it us 'any'
  return true;
}

bool base(attr_bitset attributes)
{
  // base type cannot be void
  if (attributes[ATTR_void]) return false;

  // base type cannot be an array type
  if (attributes[ATTR_array]) return false;
  
  // not void or array: its a valid base
  return true;
}

bool primitive(attr_bitset attributes)
{
  // cannot be array
  if (attributes[ATTR_array]) return false;

  // is either bool, char or int
  if (attributes[ATTR_bool] || attributes[ATTR_char]
      || attributes[ATTR_int])
    { return true; }

  // wasn't any of the above
  return false;
}

// newstring
// index
// call
// newarray
// field

struct dfs_ret
{
  attr_bitset attributes;
  string *type;
};

dfs_ret *dfs_traversal(astree* tree)
{
  // Root case
  if (tree->symbol == TOK_ROOT)
    {
      symbolStackInit();
      for(auto it = tree->children.begin();
          it < tree->children.end(); it++)
        {
          dfs_traversal(*it);
        }
    }

  // Cases where something needs to be put in a sym table
  else if (tree->symbol == TOK_STRUCT)
    { 
      symbol* test_struct = 
        find_typeTable(tree->children.at(0)->lexinfo);
      if (test_struct != NULL && test_struct->fields != NULL)
        {
          fprintf(stderr, "oc: double declaration of struct %s\n",
                  tree->children.at(0)->lexinfo->c_str());
        } else {
        if(test_struct != NULL){
          myFunctionToDoThatThing (tree);
        }
        symbol* struct_symbol;
        int blocNum;
        symbolStackPush();
        blocNum = getCurrentBloc();
        symbolStackPop();
        struct_symbol = intern_typeTable (tree);
        if (struct_symbol != NULL) {
          tree->attributes.set(ATTR_struct);
          tree->children.at(0)->attributes.set(ATTR_struct);
          fprintf(sym_file, "%s (%zu.%zu.%zu) {%zu} struct \"%s\"\n",
                  tree->children.at(0)->lexinfo->c_str(),
                  struct_symbol->filenr, struct_symbol->linenr,
                  struct_symbol->offset, struct_symbol->blocknr,
                  tree->children.at(0)->lexinfo->c_str());
          for(u_int i = 1; i < tree->children.size(); i++)
            {
              tree->children.at(i)->blocknr = blocNum;
              tree->children.at(i)->children.back()->blocknr = blocNum;
              tree->children.at(i)->children.at(0)->blocknr = blocNum;
              dfsTraversalStruct (tree->children.at(i), struct_symbol,
                                  (string *)tree->children.at(0)->
                                  lexinfo);
            }
        }
      }
    }
  else if (tree->symbol == TOK_PROTOTYPE)
    {
      symbolStackAddPrototype (tree, sym_file);
    }
  else if (tree->symbol == TOK_FUNCTION) 
    {
      int added = symbolStackAddFunction (tree, sym_file);

      if(added){
      fun_declid = tree->children.at(0)->children.back();
      for(auto it = tree->children.at(2)->children.begin();
          it < tree->children.at(2)->children.end(); it++)
        {
          dfs_traversal(*it);
        } 
      symbolStackPop ();
      }
    }
  else if (tree->symbol == TOK_VARDECL) 
    {
      // eval left, get attr and astree node of declid

      dfs_ret *left = dfs_traversal(tree->children.at(0));

      if (global_lexinfo != nullptr)
        {
          declid->type = global_lexinfo;
          global_lexinfo = nullptr;
        }
      
      tree->blocknr = getCurrentBloc();
      declid->attributes = left->attributes;
      declid->attributes.set(ATTR_variable);
      declid->attributes.set(ATTR_lval);
      declid->blocknr = getCurrentBloc();

      symbolStackAddVardecl (declid, sym_file);

      dfs_ret *right = dfs_traversal(tree->children.at(1));
      if (!compatible(declid->attributes, right->attributes,
                      declid->type, right->type))
        {
          fprintf(stderr, "oc: variable declaration of %s is assigned "
                  "an invalid type\n", declid->lexinfo->c_str());
        }
      
    }

  // Cases that do not require putting something in sym table
  else if (tree->symbol == TOK_BLOCK)
    {
      tree->blocknr = getCurrentBloc();
      symbolStackPush ();
      // increment block count
      // do every child
      
      for(auto it = tree->children.begin();
          it < tree->children.end(); it++)
        {
          dfs_traversal(*it);
        } 
      symbolStackPop ();
    }

  else if (tree->symbol == TOK_WHILE)
    {
      // first child needs to be bool
      dfs_ret *left = dfs_traversal(tree->children.at(0));
      tree->blocknr = getCurrentBloc();
      if (!left->attributes[ATTR_bool] || left->attributes[ATTR_array])
        {
          fprintf(stderr, "oc: expression in while must be a bool\n");
        }
      dfs_traversal(tree->children.at(1));
    }

  else if (tree->symbol == TOK_IF || tree->symbol == TOK_IFELSE)
    {
      // first child needs to be bool
      dfs_ret *left = dfs_traversal(tree->children.at(0));
      tree->blocknr = getCurrentBloc();
      if (!left->attributes[ATTR_bool] || left->attributes[ATTR_array])
        {
          fprintf(stderr, "oc: expression in while must be a bool\n");
        }
      // do 1 or 2 more depending on IF or IFELSE
      dfs_traversal(tree->children.at(1));
      if (tree->children.size() > 2){
        dfs_traversal(tree->children.at(2));
      }
    }

  // find me
  else if (tree->symbol == TOK_RETURN)
    {
      //CHECK CHILD AGAINST RETURN TYPE
      dfs_ret *child = dfs_traversal(tree->children.at(0));
      tree->blocknr = getCurrentBloc();
      if(child == NULL)
        {
          fprintf(stderr, "oc: returned value does not exist\n");
          return nullptr;
        }
      if(!compatible(fun_declid->attributes, child->attributes,
                     fun_declid->type, child->type))
        {
          fprintf(stderr, "oc: returned value does not match" 
                  "function return type\n");
        }
    }

  else if (tree->symbol == TOK_RETURNVOID)
    {
      //CHECK CHILD AGAINST RETURN TYPE
      tree->blocknr = getCurrentBloc();
      if (!fun_declid->attributes[ATTR_void])
        {
          fprintf(stderr, "oc: returned value does not match" 
                  "function return type\n");
        }
    }


  else if (tree->symbol == '=')
    {
      dfs_ret *left = dfs_traversal(tree->children.at(0));
      dfs_ret *right = dfs_traversal(tree->children.at(1));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;

      if(left == NULL)
        {
          fprintf(stderr, "oc: Failed to locate identifier\n");
          return retval;
        }
      if(right == NULL)
        {
          fprintf(stderr, "oc: Failed to locate identifier\n");
          return retval;
        }

      if (any(left->attributes) && left->attributes[ATTR_lval] 
          && any(right->attributes))
        {
          if (compatible(left->attributes, right->attributes,
                         left->type, right->type))
            {
              setAttributes(retval->attributes, left->attributes);
              retval->type = left->type;
              retval->attributes.set(ATTR_vreg);
            }
          else
            {
              fprintf(stderr, 
                      "oc: operands of '=' aren't compatible\n");
            }
        }
      else
        {
          fprintf(stderr, "oc: operands of '=' aren't valid\n");
        }
      return retval;
    }


  else if (tree->symbol == TOK_EQ || tree->symbol == TOK_NE)
    {
      dfs_ret *left = dfs_traversal(tree->children.at(0));
      dfs_ret *right = dfs_traversal(tree->children.at(1));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;

      if(left == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }
      if(right == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }

      char *op = NULL;
      if (tree->symbol == TOK_EQ) { op = (char*)"=="; }
      else if (tree->symbol == TOK_NE) { op = (char*)"!="; }

      if (!any(left->attributes))
        { 
          fprintf(stderr, "oc: left operand of %s is not 'any'\n", op);
        }

      if (!any(right->attributes))
        { 
          fprintf(stderr, "oc: right operand of %s is not 'any\n", op);
        }

      // both 'any'
      if (any(left->attributes) && any(right->attributes))
        {
          // compatible
          // NEED TYPEID !!!!!!!!!
          if (compatible(left->attributes, right->attributes, 
                         left->type, right->type))
            {
              tree->attributes.set(ATTR_bool);
              tree->attributes.set(ATTR_vreg);
              retval->attributes.set(ATTR_bool);
              retval->attributes.set(ATTR_vreg);
            }
          // not compatible
          else
            {
              fprintf(stderr, 
                      "oc: operands of %s not compatible\n", op);
            }
        }

      return retval;
    }

  else if (tree->symbol == TOK_LT || tree->symbol == TOK_LE
           || tree->symbol == TOK_GT || tree->symbol == TOK_GE)
    {
      dfs_ret *left = dfs_traversal(tree->children.at(0));
      dfs_ret *right = dfs_traversal(tree->children.at(1));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;

      if(left == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }
      if(right == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }

      char *op = NULL;
      if (tree->symbol == TOK_LT) { op = (char*)"<"; }
      else if (tree->symbol == TOK_LE) { op = (char*)"<="; }
      else if (tree->symbol == TOK_GT) { op = (char*)">"; }
      else if (tree->symbol == TOK_GE) { op = (char*)">="; }

      if (!primitive(left->attributes))
        { 
          fprintf(stderr, 
                  "oc: left operand of %s is not primitive\n", op);
        }

      if (!primitive(right->attributes))
        { 
          fprintf(stderr, 
                  "oc: right operand of %s is not primitive\n", op);
        }

      // both primitive
      if (primitive(left->attributes) && primitive(right->attributes))
        {
          // compatible, typeids not needed since both primitive
          if (compatible(left->attributes, right->attributes, 
                         left->type, right->type))
            {
              tree->attributes.set(ATTR_bool);
              tree->attributes.set(ATTR_vreg);
              retval->attributes.set(ATTR_bool);
              retval->attributes.set(ATTR_vreg);
            }
          // primitive but not compatible
          else
            {
              fprintf(stderr, 
                      "oc: operands of %s not compatible\n", op);
            }
        }
      else
        {
          fprintf(stderr, 
                  "oc: operands of %s are not primitive\n", op);
        }

      return retval;
    }

  else if (tree->symbol == '+' || tree->symbol == '-'
           || tree->symbol == '*' || tree->symbol == '/'
           || tree->symbol == '%')
    {
      dfs_ret *left = dfs_traversal(tree->children.at(0));
      dfs_ret *right = dfs_traversal(tree->children.at(1));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->blocknr = getCurrentBloc();

      if(left == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }
      if(right == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }

      if ((left->attributes[ATTR_int] && right->attributes[ATTR_int])
          && (!left->attributes[ATTR_array] 
              && !right->attributes[ATTR_array]))
        {
          tree->attributes.set(ATTR_int);
          tree->attributes.set(ATTR_vreg);
          retval->attributes.set(ATTR_int);
          retval->attributes.set(ATTR_vreg);
        }
      else
        {
          if (tree->symbol == '+')
            { fprintf(stderr, "oc: invalid type: binary +\n"); }
          else if (tree->symbol == '-')
            { fprintf(stderr, "oc: invalid type: binary -\n"); }
          else if (tree->symbol == '*')
            { fprintf(stderr, "oc: invalid type: *\n"); }
          else if (tree->symbol == '/')
            { fprintf(stderr, "oc: invalid type: /\n"); }
          else if (tree->symbol == '%')
            { fprintf(stderr, "oc: invalid type: %%\n"); }
        }
      return retval;
    }

  else if (tree->symbol == TOK_POS || tree->symbol == TOK_NEG)
    {
      dfs_ret *child = dfs_traversal(tree->children.at(0));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->blocknr = getCurrentBloc();

      if (child->attributes[ATTR_int] && 
          !child->attributes[ATTR_array])
        {
          tree->attributes.set(ATTR_int);
          tree->attributes.set(ATTR_vreg);
          retval->attributes.set(ATTR_int);
          retval->attributes.set(ATTR_vreg);
        }
      else
        {
          if (tree->symbol == TOK_POS) 
            { fprintf(stderr, "oc: invalid type: unary +\n"); }
          else if (tree->symbol == TOK_NEG)
            { fprintf(stderr, "oc: invalid type: unary -\n"); }
        }
      
      return retval;
    }

  else if (tree->symbol == '!')
    {
      dfs_ret *child = dfs_traversal(tree->children.at(0));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      if(child == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }
      if (child->attributes[ATTR_bool] &&
          !child->attributes[ATTR_array])
        {
          tree->attributes.set(ATTR_bool);
          tree->attributes.set(ATTR_vreg);
          retval->attributes.set(ATTR_bool);
          retval->attributes.set(ATTR_vreg);
        }
      else
        {
          fprintf(stderr, "oc: invalid type: !\n");
        }
      
      return retval;    
    }

  else if (tree->symbol == TOK_ORD)
    {
      dfs_ret *child = dfs_traversal(tree->children.at(0));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->blocknr = getCurrentBloc();

      if(child == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }

      if (child->attributes[ATTR_char] && 
          !child->attributes[ATTR_array])
        {
          tree->attributes.set(ATTR_int);
          tree->attributes.set(ATTR_vreg);
          retval->attributes.set(ATTR_int);
          retval->attributes.set(ATTR_vreg);
        }
      else
        {
          fprintf(stderr, "oc: invalid type: ord\n");
        }
      return retval;
    }

  else if (tree->symbol == TOK_CHR)
    {
      dfs_ret *child = dfs_traversal(tree->children.at(0));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->blocknr = getCurrentBloc();

      if(child == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }

      if (child->attributes[ATTR_char] && 
          !child->attributes[ATTR_array])
        {
          tree->attributes.set(ATTR_char);
          tree->attributes.set(ATTR_vreg);
          retval->attributes.set(ATTR_char);
          retval->attributes.set(ATTR_vreg);
        }
      else
        {
          fprintf(stderr, "oc: invalid type: chr\n");
        }

      return retval;
    }

  // 'new' cases
  
  // for typeid
  else if (tree->symbol == TOK_NEW)
    {
      dfs_ret *child = dfs_traversal(tree->children.at(0));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->attributes = 0;

      if(child == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }

      retval->attributes |= (child->attributes & types);
      // setAttributes(retval->attributes, child->attributes);
      retval->attributes.set(ATTR_vreg);
      retval->type = child->type;
      tree->attributes = retval->attributes;
      tree->type = retval->type;

      return retval;
    }

  // string
  else if (tree->symbol == TOK_NEWSTRING)
    {
      dfs_ret *child = dfs_traversal(tree->children.at(0));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->attributes = 0;

      if(child == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }

      // not an int or its an array
      if (child->attributes[ATTR_int] &&
          !child->attributes[ATTR_array])
        {
          retval->attributes.set(ATTR_string);
          retval->attributes.set(ATTR_vreg);
          tree->attributes = retval->attributes;
        }
      else
        {
          fprintf(stderr, "oc: expression in parenthesis of"
                  "string allocation must evaluate to an int\n");
        }

      return retval;
    }

  //  array
  else if (tree->symbol == TOK_NEWARRAY)
    {
      dfs_ret *left = dfs_traversal(tree->children.at(0));
      dfs_ret *right = dfs_traversal(tree->children.at(1));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->attributes = 0;

      if(left == NULL)
        {
          fprintf(stderr, "oc: Variable %s not declared\n",
                  tree->children.at(0)->lexinfo->c_str());
          return retval;
        }
      if(right == NULL)
        {
          fprintf(stderr, "oc: Variable %s not declared\n",
                  tree->children.at(1)->lexinfo->c_str());
          return retval;
        }

      if (base(left->attributes))
        {
          if (right->attributes[ATTR_int]
              && !right->attributes[ATTR_array])
            {
              retval->attributes |= (left->attributes & types);
              retval->type = left->type;              
              retval->attributes.set(ATTR_array);
              retval->attributes.set(ATTR_vreg);
              tree->attributes = retval->attributes;
              tree->type = retval->type;
            }
          else
            {
              fprintf(stderr, "oc: expression in brackets of"
                      "array allocation must evaluate to an int\n");
            }
        }
      else
        {
          fprintf(stderr, "oc: cannot make an array of array types\n");
        }

      return retval;
    }

  else if (tree->symbol == TOK_INDEX)
    {
      dfs_ret *left = dfs_traversal(tree->children.at(0));
      dfs_ret *right = dfs_traversal(tree->children.at(1));
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;

      if(left == NULL)
        {
          fprintf(stderr, "oc: Variable %s not declared\n", 
                  tree->children.at(0)->lexinfo->c_str());
          return retval;
        }
      if(right == NULL)
        {
          fprintf(stderr, "oc: Variable %s not declared\n", 
                  tree->children.at(1)->lexinfo->c_str());
          return retval;
        }

      // right side is int
      if (right->attributes[ATTR_int] &&
           !right->attributes[ATTR_array])
        {
          if(left->attributes[ATTR_string] &&
                  !left->attributes[ATTR_array])
            {
              retval->attributes.set(ATTR_char);
              retval->attributes.set(ATTR_vaddr);
              retval->attributes.set(ATTR_lval);
              tree->attributes = retval->attributes;
            }
          else if (left->attributes[ATTR_array])
            {
              retval->attributes |= (left->attributes & types);
              retval->attributes.flip(ATTR_array);
              retval->attributes.set(ATTR_vaddr);
              retval->attributes.set(ATTR_lval);
              retval->type = left->type;
              tree->attributes = retval->attributes;
            }
          else
            {
              fprintf(stderr, "oc: left side of index not valid\n");
            }
        }
      else
        {
          fprintf(stderr, 
                  "oc: index of an array/string must be an int\n");
        }
      return retval;
    }

  // ...
  else if (tree->symbol == '.')
    {
      // ident
      dfs_ret *left = dfs_traversal(tree->children.at(0));
      // field
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      symbol *fieldptr = nullptr;

      if(left == NULL)
        {
          fprintf(stderr, "oc: Failed to locate symbol\n");
          return retval;
        }

      if (left->attributes[ATTR_struct] && 
          !left->attributes[ATTR_array])
        {
          
          symbol *selector_struct = grabStructSymbol(left->type);
          if (nullptr != selector_struct)
            {

              auto table_check = 
                selector_struct->fields->find
                (tree->children.at(1)->lexinfo);
              if (selector_struct->fields->end() == table_check)
                {
                  fprintf(stderr, 
                          "oc: didn't find given field in struct\n");
                }
              else
                {
                  // set current node stuff
                  fieldptr = table_check->second;
                  retval->attributes |= 
                    (fieldptr->attributes & types);
                  retval->attributes.set(ATTR_vaddr);
                  retval->attributes.set(ATTR_lval);
                  retval->type = fieldptr->type_id;
                  tree->attributes = retval->attributes;
                  tree->type = retval->type;

                  // set field node stuff
                  tree->children.at(1)->attributes =
                    fieldptr->attributes;
                }
            }
          else
            {
              fprintf(stderr, "oc: type does not exist\n");
            }
          
        }
      else
        {
          fprintf(stderr, "oc: left side of field selector must"
                  "be a struct type");
        }
      return retval;
    }


  else if (tree->symbol == TOK_DECLID)
    {
      declid = tree;
      tree->blocknr = getCurrentBloc();
      return NULL;
    }
  
  // array

  else if (tree->symbol == TOK_ARRAY)
    {
      dfs_ret *retval = dfs_traversal(tree->children.at(0));
      dfs_traversal(tree->children.at(1));
      tree->blocknr = getCurrentBloc();

      tree->attributes.set(ATTR_array);
      retval->attributes.set(ATTR_array);
      
      return retval;
    }

  // types

  else if (tree->symbol == TOK_VOID)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->blocknr = getCurrentBloc();
      
      retval->attributes.set(ATTR_void);
      tree->attributes.set(ATTR_void);

      if (!tree->children.empty())
        {
          dfs_traversal(tree->children.at(0));
        }

      return retval;
    }

  else if (tree->symbol == TOK_BOOL)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->blocknr = getCurrentBloc();
      
      retval->attributes.set(ATTR_bool);
      tree->attributes.set(ATTR_bool);
      if (!tree->children.empty())
        {
          dfs_traversal(tree->children.at(0));
        }
      return retval;
    }

  else if (tree->symbol == TOK_CHAR)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->blocknr = getCurrentBloc();
      
      retval->attributes.set(ATTR_char);
      tree->attributes.set(ATTR_char);
      if (!tree->children.empty())
        {
          dfs_traversal(tree->children.at(0));
        }

      return retval;
    }

  else if (tree->symbol == TOK_INT)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->blocknr = getCurrentBloc();
      
      retval->attributes.set(ATTR_int);
      tree->attributes.set(ATTR_int);
      if (!tree->children.empty())
        {
          dfs_traversal(tree->children.at(0));
        }
      
      return retval;
    }

  else if (tree->symbol == TOK_STRING)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;
      tree->blocknr = getCurrentBloc();
      
      retval->attributes.set(ATTR_string);
      tree->attributes.set(ATTR_string);
      if (!tree->children.empty())
        {
          dfs_traversal(tree->children.at(0));
        }

      return retval;
    }

  else if (tree->symbol == TOK_TYPEID)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->type = (string *)tree->lexinfo;
      retval->attributes = 0;
      retval->attributes.set(ATTR_struct);
      tree->type = (string *)tree->lexinfo;
      global_lexinfo = tree->type;
      tree->attributes.set(ATTR_struct);
      if (!tree->children.empty())
        {
          dfs_traversal(tree->children.at(0));
        }
      
      return retval;
    }

  else if (tree->symbol == TOK_CHARCON)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;
      retval->attributes.set(ATTR_const);
      retval->attributes.set(ATTR_char);
      tree->attributes.set(ATTR_const);
      tree->attributes.set(ATTR_char);
      
      return retval;
    }

  else if (tree->symbol == TOK_STRINGCON)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;
      retval->attributes.set(ATTR_const);
      retval->attributes.set(ATTR_string);
      tree->attributes.set(ATTR_const);
      tree->attributes.set(ATTR_string);

      return retval;
    }

  else if (tree->symbol == TOK_INTCON)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;
      retval->attributes.set(ATTR_const);
      retval->attributes.set(ATTR_int);
      tree->attributes.set(ATTR_const);
      tree->attributes.set(ATTR_int);

      return retval;
    }

  else if (tree->symbol == TOK_CHARCON)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;
      retval->attributes.set(ATTR_const);
      retval->attributes.set(ATTR_char);
      tree->attributes.set(ATTR_const);
      tree->attributes.set(ATTR_char);

      return retval;
    }

  else if (tree->symbol == TOK_STRINGCON)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;
      retval->attributes.set(ATTR_const);
      retval->attributes.set(ATTR_string);
      tree->attributes.set(ATTR_const);
      tree->attributes.set(ATTR_string);

      return retval;
    }

  else if (tree->symbol == TOK_TRUE || tree->symbol == TOK_FALSE)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;
      retval->attributes.set(ATTR_const);
      retval->attributes.set(ATTR_bool);
      tree->attributes.set(ATTR_const);
      tree->attributes.set(ATTR_bool);

      return retval;
    }

  else if (tree->symbol == TOK_IDENT)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      retval->attributes = 0;

      symbol* ident_stuff = getInfo(tree->lexinfo);
      if(ident_stuff == NULL)
        {
          fprintf(stderr, "oc: ident %s not found\n", 
                   tree->lexinfo->c_str());
          return NULL;
        }
      tree->attributes = ident_stuff->attributes;
      tree->blocknr = ident_stuff->blocknr;
      tree->decfilenr = ident_stuff->filenr;
      tree->declinenr = ident_stuff->linenr;
      tree->decoffset = ident_stuff->offset;
      tree->printdecs = 1;
      retval->attributes = ident_stuff->attributes;

      if (tree->attributes.test(ATTR_struct))
        {
          tree->type = ident_stuff->type_id;
          retval->type = ident_stuff->type_id;
        }

      return retval;
    }

  else if (tree->symbol == TOK_CALL)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      for(u_int i = 0; i < tree->children.size(); i++)
        {
          dfs_traversal(tree->children.at(i));
        }
      symbol* temp_symb = grabFunctionSymbol(tree->children.at(0));
      if(temp_symb == NULL)
        {
          retval->attributes = 0;
          return retval;
        }
      if (!compatible(temp_symb->attributes,
                      tree->children.at(0)->attributes,
                      temp_symb->type_id, tree->children.at(0)->type))
        {
          fprintf(stderr, "oc: Error with function type declaration\n");
        }
      for(u_int k = 1; k < tree->children.size(); k++)
        {
          if (!compatible(temp_symb->parameters->at(k-1)->attributes,
                          tree->children.at(k)->attributes,
                          temp_symb->parameters->at(k-1)->type_id,
                          tree->children.at(k)->type))
            {
              fprintf(stderr,
                      "oc: Invalid argument sent into function %s\n",
                      tree->children.at(0)->lexinfo->c_str());
            }
        } 
      tree->decfilenr = temp_symb->filenr;
      tree->declinenr = temp_symb->linenr;
      tree->decoffset = temp_symb->offset;
      tree->printdecs = 1;
      retval->attributes = tree->children.at(0)->attributes;
      retval->type = tree->children.at(0)->type;
      return retval;
    }

  else if (tree->symbol == TOK_NULL)
    {
      dfs_ret *retval = (dfs_ret *)malloc(sizeof(*retval));
      tree->blocknr = getCurrentBloc();
      retval->attributes = 0;
      retval->attributes.set(ATTR_const);
      retval->attributes.set(ATTR_null);
      tree->attributes.set(ATTR_const);
      tree->attributes.set(ATTR_null);

      return retval;

    }

    return nullptr;
}

void dfsTraversal(astree* tree)
{
  types.set(ATTR_bool);
  types.set(ATTR_char);
  types.set(ATTR_int);
  types.set(ATTR_null);
  types.set(ATTR_string);
  types.set(ATTR_struct);
  types.set(ATTR_array);

  dfs_traversal(tree);
}
