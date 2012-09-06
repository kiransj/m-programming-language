#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <stdio.h>

extern unsigned int line_number;

char* token_to_str(int token);
typedef enum
{
	IDENTIFIER_TYPE_UNKNOWN,
	IDENTIFIER_TYPE_NUMBER,
	IDENTIFIER_TYPE_FLOAT,	
	IDENTIFIER_TYPE_STRING,
	IDENTIFIER_TYPE_ARGUMENT,
	IDENTIFIER_TYPE_VARIABLE,
	IDENTIFIER_TYPE_REGISTER,
}IdentifierType;

char* IdentifierType_str(IdentifierType);

typedef struct _Identifier
{
	IdentifierType type;
	union
	{
		char 	*str;
		char 	*variable_name;
		double 	real;
		int 	number;
		int 	argument_number;
		int		register_number;
	}u;
}*Identifier;

void Identifier_to_str(Identifier id, char * const buffer, const int size);


Identifier Identifier_Create(void);
void Identifier_Destroy(Identifier A);
Identifier Identifier_Clone(Identifier A);
Identifier Identifier_NewString(const char *str);
Identifier Identifier_NewFloat(const double real);
Identifier Identifier_NewArgument(const int argument_number);
Identifier Identifier_NewVariable(const char *variable_name);
Identifier Identifier_NewInteger(const int number);
Identifier Identifier_NewRegister(const int number);
#ifndef YYSTYPE
#define YYSTYPE Identifier 
#endif

YYSTYPE   yylval;

extern FILE *yyin;

int yylex(void);
int yylex_destroy(void);
#endif

