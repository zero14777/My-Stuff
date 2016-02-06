#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astree.h"
#include "symboltable.h"
#include "stringset.h"
#include "lyutils.h"

astree::astree (attr_bitset attributes, int blocknr,
                struct symbol* struct_table_node,
                int symbol, int filenr, int linenr,
                int offset, int decfilenr, int declinenr,
                int decoffset, int printdecs, 
                const char* clexinfo, string* type):
  type (type), attributes (attributes), blocknr (blocknr),
  struct_table_node (struct_table_node),
  symbol (symbol), filenr (filenr), linenr (linenr),
  offset (offset), decfilenr (decfilenr),
  declinenr (declinenr),
  decoffset (decoffset), printdecs (printdecs), 
  lexinfo (intern_stringset (clexinfo))
{
  DEBUGF ('f', "astree %p->{%d:%d.%d: %s: \"%s\"}\n",
          (void*) this, filenr, linenr, offset,
          get_yytname (symbol), lexinfo->c_str());
}

astree* adopt1 (astree* root, astree* child) {
  root->children.push_back (child);
  DEBUGF ('a', "%p (%s) adopting %p (%s)\n",
          root, root->lexinfo->c_str(),
          child, child->lexinfo->c_str());
  return root;
}

astree* adopt2 (astree* root, astree* left, astree* right) {
  adopt1 (root, left);
  adopt1 (root, right);
  return root;
}

astree* adopt3 (astree* root, astree* first,
                astree* second, astree* third) {
  adopt1 (root, first);
  adopt1 (root, second);
  adopt1 (root, third);
  return root;
}

astree* adopt1sym (astree* root, astree* child, int symbol) {
  adopt1 (root, child);
  root->symbol = symbol;
  return root;
}

astree* changeSym (astree* root, int symbol) {
  root->symbol = symbol;
  return root;
}
char* trim_yytname (const char *yytname) {
  if (strstr (yytname, "TOK_") == yytname) yytname += 4;
  return (char *)yytname;
}

static void dump_node (FILE* outfile, astree* node) {
  fprintf (outfile, "%s \"%s\" (%ld.%ld.%ld) {%d} ",
           trim_yytname(get_yytname (node->symbol)),
           node->lexinfo->c_str(), node->filenr,
           node->linenr, node->offset, node->blocknr);
  print_attrs(outfile, node->attributes, node->type, NULL);
  if(node->printdecs)
    {
      fprintf(outfile, "(%ld.%ld.%ld) ", node->decfilenr,
                        node->declinenr, node->decoffset);
    }
  bool need_space = false;
  for (size_t child = 0; child < node->children.size();
       ++child) {
    if (need_space) fprintf (outfile, " ");
    need_space = true;
    /* fprintf (outfile, "%p", node->children.at(child)); */
  }
  /* fprintf (outfile, "]}"); */

}

static void dump_astree_rec (FILE* outfile, astree* root,
                             int depth) {
  if (root == NULL) return;
  /*
    fprintf (outfile, "%*s%s ", depth * 3, "",
    root->lexinfo->c_str());
  */
  for (int i = depth; i > 0; --i)
    {
      fprintf (outfile, "|  ");
    }
  //   fprintf (outfile, "%*s ", depth * 3, "");

  dump_node (outfile, root);
  fprintf (outfile, "\n");
  for (size_t child = 0; child < root->children.size();
       ++child) {
    dump_astree_rec (outfile, root->children[child],
                     depth + 1);
  }
}

void dump_astree (FILE* outfile, astree* root) {
  dump_astree_rec (outfile, root, 0);
  fflush (NULL);
}

void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep) {
  if (is_defined_token (toknum)) {
    dump_node (outfile, yyvaluep);
  }else {
    fprintf (outfile, "%s(%d)\n",
             get_yytname (toknum), toknum);
  }
  fflush (NULL);
}

void free_ast (astree* root) {
  while (not root->children.empty()) {
    astree* child = root->children.back();
    root->children.pop_back();
    free_ast (child);
  }
  DEBUGF ('f', "free [%p]-> %d:%d.%d: %s: \"%s\")\n",
          root, root->filenr, root->linenr, root->offset,
          get_yytname (root->symbol), root->lexinfo->c_str());
  delete root;
}

void free_ast2 (astree* tree1, astree* tree2) {
  free_ast (tree1);
  free_ast (tree2);
}

static void print_attr(FILE *out, string attr, string *extra)
{
  fprintf(out, "%s ", attr.c_str());
  if (attr == "field" && nullptr != extra)
    {
      fprintf(out, "{%s} ", extra->c_str());
    }
  else if (attr == "struct" && nullptr != extra)
    {
      fprintf(out, "\"%s\" ", extra->c_str());
    }
   
}

void print_attrs (FILE *out, attr_bitset attributes,
                  string *type, string *fieldof)
{
  for (size_t i = 0; i < attributes.size(); ++i)
    {
      if (attributes.test(i))
        {
          switch (i)
            {
            case ATTR_void:
              print_attr(out, "void", nullptr);
              break;
            case ATTR_bool:
              print_attr(out, "bool", nullptr);
              break;
            case ATTR_char:
              print_attr(out, "char", nullptr);
              break;
            case ATTR_int:
              print_attr(out, "int", nullptr);
              break;
            case ATTR_null:
              print_attr(out, "null", nullptr);
              break;
            case ATTR_string:
              print_attr(out, "string", nullptr);
              break;
            case ATTR_struct:
              print_attr(out, "struct", type);
              break;
            case ATTR_array:
              print_attr(out, "array", nullptr);
              break;
            case ATTR_function:
              print_attr(out, "function", nullptr);
              break;
            case ATTR_variable:
              print_attr(out, "variable", nullptr);
              break;
            case ATTR_field:
              print_attr(out, "field", fieldof);
              break;
            case ATTR_typeid:
              print_attr(out, "typeid", nullptr);
              break;
            case ATTR_param:
              print_attr(out, "param", nullptr);
              break;
            case ATTR_lval:
              print_attr(out, "lval", nullptr);
              break;
            case ATTR_const:
              print_attr(out, "const", nullptr);
              break;
            case ATTR_vreg:
              print_attr(out, "vreg", nullptr);
              break;
            case ATTR_vaddr:
              print_attr(out, "vaddr", nullptr);
              break;
            default:
              break;
            }
        }
    }
}
