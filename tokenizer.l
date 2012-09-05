%{
#include <string.h>
#include <stdlib.h>
#include "tokenzier.h"

%}

%%
"unknown"		{   return KEYWORD_UNKNOWN;}
"string"		{   return KEYWORD_STRING;}
"int"			{   return KEYWORD_INT;}
"real"			{   return KEYWORD_FLOAT;}

"var"			{   return KEYWORD_VAR;}
"return"		{   return KEYWORD_RETURN;}
"while"	   		{   return KEYWORD_WHILE; }
"endwhile"  	{   return KEYWORD_ENDWHILE;}
"if"	   		{   return KEYWORD_IF; }
"endif"  		{   return KEYWORD_ENDIF;}
"function"  	{   return KEYWORD_FUNCTION;}
"endfunction"   {   return KEYWORD_ENDFUNCTION;}

\$[0-9]+			   { yylval = Identifier_NewArgument(atoi(&yytext[1])); return TOKEN_TYPE_INDENTIFIER;}
[0-9]+				   { yylval = Identifier_NewInteger(atoi(yytext)); 	  	return TOKEN_TYPE_INDENTIFIER;}
[0-9]*\.[0-9]+		   { yylval = Identifier_NewFloat(atof(yytext)); 		return TOKEN_TYPE_INDENTIFIER;}
\"[^\"]*\"			   { yylval = Identifier_NewString(yytext); 			return TOKEN_TYPE_INDENTIFIER;}
[a-zA-Z_][a-zA-Z_0-9]* { yylval = Identifier_NewVariable(yytext); 		  	return TOKEN_TYPE_VARIABLE;}

"+="	{  return OPERATOR_ADDEQ;}
"-="	{  return OPERATOR_SUBEQ;}
"+"		{  return OPERATOR_ADD;}
"-"		{  return OPERATOR_SUB;}
"*"		{  return OPERATOR_MUL;}
"/"		{  return OPERATOR_DIV;}
">"		{  return OPERATOR_GTH;}
"<"		{  return OPERATOR_LTH;}
"="		{  return OPERATOR_SET;}
"^"	 	{  return OPERATOR_POW;}
"!"		{  return OPERATOR_NOT;}
"%"		{  return OPERATOR_MOD;}
">="	{  return OPERATOR_GTE;}
"<="	{  return OPERATOR_LTE;}
"=="	{  return OPERATOR_CMP;}
"!="	{  return OPERATOR_NEQ;}
"||" 	{  return OPERATOR_OR; }
"&&"	{  return OPERATOR_AND;}
";"		{  return OPERATOR_SEMI_COLON;}
","		{  return OPERATOR_COMMA;}
"("		{  return OPERATOR_OPEN_PAREN;}
")"		{  return OPERATOR_CLOSE_PAREN;}

\n		{line_number++;}
[\t ]*	;
#[^\n]*	;
.		{fprintf(stderr, "unknown string %s on line %d\n", yytext, line_number);   return 0;}
%%

void yyerror(char *s)
{
	fprintf(stderr, "error %s\n", s);
}

int yywrap(void)
{
	return 1;
}
