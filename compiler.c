#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "compiler.h"
#include "tokenizer.h"
#include "util.h"

#include "grammer.h"

unsigned int line_number;

int lexer_to_grammer(TokenType rv)
{
	switch(rv)
	{
		case	OPERATOR_EQU:			return OPER_EQU;
		case	OPERATOR_AND:			return OPER_AND;	
		case	OPERATOR_OR:			return OPER_OR;
		case	OPERATOR_CMP:			return OPER_CMP;
		case	OPERATOR_NEQ:			return OPER_NEQ;
		case	OPERATOR_GTH:			return OPER_GTH;
		case	OPERATOR_GTE:			return OPER_GTE;
		case	OPERATOR_LTH:			return OPER_LTH;
		case	OPERATOR_LTE:			return OPER_LTE;
		case	OPERATOR_ADD:			return OPER_ADD;
		case	OPERATOR_SUB:			return OPER_SUB;
		case	OPERATOR_DIV:			return OPER_DIV;
		case	OPERATOR_MUL:			return OPER_MUL;
		case	OPERATOR_MOD:			return OPER_MOD;
		case	OPERATOR_NOT:			return OPER_NOT;
#if 0										
		case	OPERATOR_SEMI_COLON:	return OPER_SEMI_COLON;
		case	OPERATOR_COMMA:			return OPER_COMMA;
		case	OPERATOR_OPEN_PAREN:	return OPER_OPEN_PAREN;
		case	OPERATOR_CLOSE_PAREN:	return OPER_CLOSE_PAREN;

		case	KEYWORD_ENDIF:			return KW_ENDIF;
		case	KEYWORD_IF:				return KW_IF;
		case	KEYWORD_WHILE:			return KW_WHILE;
		case	KEYWORD_ENDWHILE:		return KW_ENDWHILE;
		case	KEYWORD_RETURN:			return KW_RETURN;
		case	KEYWORD_ENDFUNCTION:	return KW_ENDFUNCTION;
		case	KEYWORD_VAR:			return KW_RETURN;
		case	KEYWORD_FUNCTION:		return KW_FUNCTION;
		case	KEYWORD_INT:			return KW_INT;
		case	KEYWORD_FLOAT:			return KW_FLOAT;
		case	KEYWORD_STRING:			return KW_STRING;
		case	KEYWORD_UNKNOWN:		return KW_UNKNOWN;
		case	TOKEN_TYPE_INDENTIFIER: return TOKEN_IDENTIFIER;										
		case	TOKEN_TYPE_VARIABLE:	return TOKEN_VARIABLE;
#endif										
	}
}

int main(int argc, char *argv[])
{
	int yv;
	void *pParser;
	Compiler c;

	if(argc == 1)
	{
		LOG_INFO_NL("usage %s filename", argv[0]);
		return 1;
	}

	yyin = fopen(argv[1], "r");
	if(NULL == yyin)
	{
		printf("unable to open file\n");
		return 1;
	}

	pParser = (void*)ParseAlloc(Malloc);
	while((yv=yylex()) != 0)
	{
		c.line_number = line_number;
		Parse(pParser, lexer_to_grammer(yv), yylval, c);
		if(c.error_flag == 1)
		{
			fprintf(stderr, "stopping parsing due to syntax error\n");
			break;
		}
	}
	Parse(pParser, 0, yylval, c);
	ParseFree(pParser, Free);
	fclose(yyin);
	yylex_destroy();
	return 0;
}
