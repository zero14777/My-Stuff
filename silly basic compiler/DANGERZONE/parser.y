%{

#include "lyutils.h"
#include "astree.h"

%}

%debug
%defines
%error-verbose
%token-table
%verbose

%token TOK_VOID TOK_BOOL TOK_CHAR TOK_INT TOK_STRING
%token TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_STRUCT
%token TOK_FALSE TOK_TRUE TOK_NULL TOK_NEW TOK_ARRAY
%token TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%token TOK_IDENT TOK_INTCON TOK_CHARCON TOK_STRINGCON

%token TOK_DECLID TOK_FUNCTION TOK_PROTOTYPE TOK_PARAMLIST 
%token TOK_VARDECL TOK_RETURNVOID TOK_NEWSTRING TOK_INDEX

%token TOK_BLOCK TOK_CALL TOK_IFELSE TOK_INITDECL
%token TOK_POS TOK_NEG TOK_NEWARRAY TOK_TYPEID TOK_FIELD
%token TOK_ORD TOK_CHR TOK_ROOT

%right TOK_ELSE
%right '='
%left  TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%left  '+' '-'
%left  '*' '/' '%'
%right TOK_POS TOK_NEG '!' TOK_NEW TOK_ORD TOK_CHR

%start start

%%

start    : program            { yyparse_astree = $1; }
         ;

program  : program structdef  { $$ = adopt1 ($1, $2); }
         | program function   { $$ = adopt1 ($1, $2); }
         | program prototype  { $$ = adopt1 ($1, $2); }
         | program statement  { $$ = adopt1 ($1, $2); }
         | program error '}'  { $$ = $1; 
                                free_ast($3); }
         | program error ';'  { $$ = $1; 
                                free_ast($3); }
         |                    { $$ = new_parseroot(); }
         ;

 /* For structs */
structdef : TOK_STRUCT TOK_IDENT '{' '}'
                              { $$ = adopt1 ($1, $2);
                                changeSym ($2, TOK_TYPEID);
                                free_ast2($3, $4); }
          | manyfdecl '}'
                              { $$ = $1;
                                free_ast($2); }
          ;

manyfdecl : TOK_STRUCT TOK_IDENT '{' fielddecl ';'
                              { $$ = adopt2($1, $2, $4);
                                changeSym ($2, TOK_TYPEID);
                                free_ast2($3, $5); }
          | manyfdecl fielddecl ';'
                              { $$ = adopt1($1, $2);
                                free_ast($3); }
          ;

fielddecl : basetype TOK_IDENT
                              { $$ = $1;
                                adopt1($1, $2);
                                changeSym ($2, TOK_FIELD); }
          | basetype TOK_ARRAY TOK_IDENT
                              { $$ = adopt2($2, $1, $3);
                                changeSym ($3, TOK_FIELD); }
          ;
 /* End for structs */

basetype  : TOK_VOID          { $$ = $1; }
          | TOK_BOOL          { $$ = $1; }
          | TOK_CHAR          { $$ = $1; }
          | TOK_INT           { $$ = $1; }
          | TOK_STRING        { $$ = $1; }
          | TOK_IDENT         { $$ = changeSym ($1, TOK_TYPEID); }
          ;

/* {...} vs ; problem */
function  : identdecl funparams ')' brcblock
                              { $$ = new_parsefunction($1);
                                adopt3($$, $1, $2, $4);
                                free_ast($3); }
          ;

prototype : identdecl funparams ')' ';'
                              { $$ = new_parseprototype($1);
                                adopt2($$, $1, $2);
                                free_ast2($3, $4); }
          ;
funparams : '('
                              { $$ = changeSym($1, TOK_PARAMLIST); }
          | funparams identdecl
                              { $$ = adopt1($1, $2); }
          | funparams ',' identdecl
                              { $$ = adopt1($1, $3);
                                free_ast($2); }
          ;

identdecl : basetype TOK_IDENT 
                              { $$ = adopt1
                                ($1, changeSym ($2, TOK_DECLID)); }
          | basetype TOK_ARRAY TOK_IDENT
                              { $$ = adopt2
                                ($2, $1, changeSym ($3, TOK_DECLID)); }
          ;

block     : brcblock          { $$ = $1; }
          | ';'               { $$ = changeSym($1, TOK_BLOCK); }

brcblock  : inblock '}'       { $$ = $1;
                                free_ast($2); }
          ;

inblock   : '{'               { $$ = changeSym($1, TOK_BLOCK); }
          | inblock statement { $$ = adopt1($1, $2); }
          ;

statement : block             { $$ = $1; }
          | vardecl           { $$ = $1; }
          | while             { $$ = $1; }
          | ifelse            { $$ = $1; }
          | return            { $$ = $1; }
          | expr ';'          { $$ = $1;
                                free_ast($2); }
          ;

vardecl   : identdecl '=' expr ';' 
                              { $$ = adopt2
                                (changeSym($2, TOK_VARDECL), $1, $3);
                                free_ast($4); }
          ;

while     : TOK_WHILE '(' expr ')' statement 
                              { $$ = adopt2($1, $3, $5);
                                free_ast2($2, $4); }
          ;

ifelse    : TOK_IF '(' expr ')' statement TOK_ELSE statement
                              { $$ = adopt3
                                (changeSym($1, TOK_IFELSE),
                                $3, $5, $7);
                                free_ast2($2, $4);
                                free_ast($6); }
          | TOK_IF '(' expr ')' statement %prec TOK_ELSE
                              { $$ = adopt2($1, $3, $5); 
                                free_ast2($2, $4); }
          ;

return    : TOK_RETURN expr ';' 
                              { $$ = adopt1($1, $2);
                                free_ast($3); }
          | TOK_RETURN ';'    
                              { $$ = changeSym($1, TOK_RETURNVOID);
                                free_ast($2); }
          ;

expr      : expr '=' expr     { $$ = adopt2($2, $1, $3); }
          | expr TOK_EQ expr  { $$ = adopt2($2, $1, $3); }
          | expr TOK_NE expr  { $$ = adopt2($2, $1, $3); }
          | expr TOK_LT expr  { $$ = adopt2($2, $1, $3); }
          | expr TOK_LE expr  { $$ = adopt2($2, $1, $3); }
          | expr TOK_GT expr  { $$ = adopt2($2, $1, $3); }
          | expr TOK_GE expr  { $$ = adopt2($2, $1, $3); }
          | expr '+' expr     { $$ = adopt2($2, $1, $3); }
          | expr '-' expr     { $$ = adopt2($2, $1, $3); }
          | expr '*' expr     { $$ = adopt2($2, $1, $3); }
          | expr '/' expr     { $$ = adopt2($2, $1, $3); }
          | expr '%' expr     { $$ = adopt2($2, $1, $3); }
          | '+' expr %prec TOK_POS
                              { $$ = adopt1
                                (changeSym($1, TOK_POS), $2); }
          | '-' expr %prec TOK_NEG
                              { $$ = adopt1
                                (changeSym($1, TOK_NEG), $2); }
          | '!' expr          { $$ = adopt1($1, $2); }
          | TOK_NEW expr      { $$ = adopt1($1, $2); }
          | TOK_ORD expr      { $$ = adopt1($1, $2); }
          | TOK_CHR expr      { $$ = adopt1($1, $2); }
          | allocator         { $$ = $1; }
          | call              { $$ = $1; }
          | '(' expr ')'      { $$ = $2; free_ast2($1, $3); }
          | variable          { $$ = $1; }
          | constant          { $$ = $1; }
          ;

allocator : TOK_NEW TOK_IDENT '(' ')'        
                              { $$ = adopt1
                                ($1, changeSym($2, TOK_TYPEID));
                                free_ast2($3, $4); }
          | TOK_NEW TOK_STRING '(' expr ')'
                              { $$ = adopt1
                                (changeSym($1, TOK_NEWSTRING), $4);
                                free_ast2($3, $5); }
          | TOK_NEW basetype '[' expr ']' 
                              { $$ = adopt2
                                (changeSym($1, TOK_NEWARRAY), $2, $4);
                                free_ast2($3, $5); }
          ;

call      : TOK_IDENT '(' ')'
                              { $$ = adopt1
                                (changeSym($2, TOK_CALL), $1);
                                free_ast($3); }
          | funargs ')'
                              { $$ = $1;
                                free_ast($2); }
          ;

funargs   : TOK_IDENT '(' expr
                              { $$ = adopt2
                                (changeSym($2, TOK_CALL), $1, $3); }
          | funargs ',' expr
                              { $$ = adopt1($1, $3);
                              free_ast($2); }
          ;

variable  : TOK_IDENT         { $$ = $1; }
          | expr '[' expr ']'
                              { $$ = adopt2
                                (changeSym($2, TOK_INDEX), $1, $3);
                                free_ast($4); }
          | expr '.' TOK_IDENT
                              { $$ = adopt2
                                ($2, $1, changeSym($3, TOK_FIELD)); }
          ;

constant  : TOK_INTCON        { $$ = $1; }
          | TOK_CHARCON       { $$ = $1; }
          | TOK_STRINGCON     { $$ = $1; }
          | TOK_FALSE         { $$ = $1; }
          | TOK_TRUE          { $$ = $1; }
          | TOK_NULL          { $$ = $1; }
          ;

%%

const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}

bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}
