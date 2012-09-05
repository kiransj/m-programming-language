#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

extern unsigned int line_number;
typedef enum
{
	OPERATOR_SET,
	OPERATOR_ADDEQ,
	OPERATOR_SUBEQ,
	OPERATOR_AND,
	OPERATOR_OR,
	OPERATOR_CMP,
	OPERATOR_NEQ,
	OPERATOR_GTH,
	OPERATOR_GTE,
	OPERATOR_LTH,
	OPERATOR_LTE,
	OPERATOR_ADD,
	OPERATOR_SUB,
	OPERATOR_DIV,
	OPERATOR_MUL,
	OPERATOR_MOD,
	OPERATOR_POW,
	OPERATOR_NOT,
	OPERATOR_SEMI_COLON,
	OPERATOR_COMMA,	
	OPERATOR_OPEN_PAREN,
	OPERATOR_CLOSE_PAREN,

	KEYWORD_ENDIF,
	KEYWORD_IF,
	KEYWORD_WHILE,
	KEYWORD_ENDWHILE,
	KEYWORD_RETURN,
	KEYWORD_ENDFUNCTION,
	KEYWORD_VAR,
	KEYWORD_FUNCTION,
	KEYWORD_INT,
	KEYWORD_FLOAT,
	KEYWORD_STRING,
	KEYWORD_UNKNOWN,

	TOKEN_TYPE_INDENTIFIER, /*Used for Integer, float, string*/
	TOKEN_TYPE_Variable,
}TokenType;

typedef enum
{
	IDENTIFIER_UNKNOWN,
	IDENTIFIER_TYPE_NUMBER,
	IDENTIFIER_TYPE_FLOAT,	
	IDENTIFIER_TYPE_STRING,
	IDENTIFIER_TYPE_ARGUMENT,
	IDENTIFIER_TYPE_Variable,
}IdentifierType;

typedef struct _Identifier
{
	IdentifierType type;
	union
	{
		char 	*str;
		char 	*variable_name;
		double 	real;
		int 	number;
		int 	argument_number
	}u;
}Identifier;

void Varable_Destroy(Identifier t);
Identifier Identifier_NewString(const char *str);
Identifier Identifier_NewFloat(const double real);
Identifier Identifier_NewArgument(const int argument_number);
Identifier Identifier_NewVariable(const char *variable_name);
Identifier Identifier_NewInteger(const int number);
#endif
