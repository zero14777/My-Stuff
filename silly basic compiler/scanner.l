%{

#include "auxlib.h"
#include "lyutils.h"

#define YY_USER_ACTION  { lexer_useraction (); }

%}

%option 8bit
%option debug
%option nodefault
%option nounput
%option noyywrap
%option verbose
%option warn

LETTER           [A-Za-z_]
DIGIT            [0-9]
INTCON           ({DIGIT}+)
IDENT            ({LETTER}({LETTER}|{DIGIT})*)
INVALIDIDENT     ({DIGIT}({LETTER}|{DIGIT})*)
CHAR             [^\\"\n]
ESCCHAR          (\\[\\'"0nt])
INVALIDESCCHAR   (\\[^\\'"0nt]?)
INVALIDCHARCON   ('({CHAR}|{ESCCHAR}|{INVALIDESCCHAR})?'?)
CHARCON          ('({CHAR}|{ESCCHAR})')
INVALIDSTR       (\"({CHAR}|{ESCCHAR}|{INVALIDESCCHAR})*\"?)
STRINGCON        (\"({CHAR}|{ESCCHAR})*\")

%%

"#".*            { lexer_include(tok_file); }
[ \t]+           { }
\n               { lexer_newline(); }

"void"           { return yylval_token (TOK_VOID); }
"bool"           { return yylval_token (TOK_BOOL); }
"char"           { return yylval_token (TOK_CHAR); }
"int"            { return yylval_token (TOK_INT); }
"string"         { return yylval_token (TOK_STRING); }
"struct"         { return yylval_token (TOK_STRUCT); }
"if"             { return yylval_token (TOK_IF); }
"else"           { return yylval_token (TOK_ELSE); }
"while"          { return yylval_token (TOK_WHILE); }
"return"         { return yylval_token (TOK_RETURN); }
"false"          { return yylval_token (TOK_FALSE); }
"true"           { return yylval_token (TOK_TRUE); }
"null"           { return yylval_token (TOK_NULL); }
"ord"            { return yylval_token (TOK_ORD); }
"chr"            { return yylval_token (TOK_CHR); }
"new"            { return yylval_token (TOK_NEW); }

{INTCON}         { return yylval_token (TOK_INTCON); }
{IDENT}          { return yylval_token (TOK_IDENT); }
{CHARCON}        { return yylval_token (TOK_CHARCON); }
{STRINGCON}      { return yylval_token (TOK_STRINGCON); }
{INVALIDIDENT}   { lexer_badtoken (yytext); }
{INVALIDCHARCON} { lexer_badtoken (yytext); }
{INVALIDSTR}     { lexer_badtoken (yytext); }

"[]"             { return yylval_token (TOK_ARRAY); }
"=="             { return yylval_token (TOK_EQ); }
"!="             { return yylval_token (TOK_NE); }
"<="             { return yylval_token (TOK_LE); }
">="             { return yylval_token (TOK_GE); }
"("              { return yylval_token ('('); }
")"              { return yylval_token (')'); }
"["              { return yylval_token ('['); }
"]"              { return yylval_token (']'); }
"{"              { return yylval_token ('{'); }
"}"              { return yylval_token ('}'); }
";"              { return yylval_token (';'); }
","              { return yylval_token (','); }
"."              { return yylval_token ('.'); }
"="              { return yylval_token ('='); }
"<"              { return yylval_token (TOK_LT); }
">"              { return yylval_token (TOK_GT); }
"+"              { return yylval_token ('+'); }
"-"              { return yylval_token ('-'); }
"*"              { return yylval_token ('*'); }
"/"              { return yylval_token ('/'); }
"%"              { return yylval_token ('%'); }
"!"              { return yylval_token ('!'); }

.                { lexer_badchar (*yytext); }

%%



