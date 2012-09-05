#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "compiler.h"
#include "tokenizer.h"
#include "util.h"

#include "grammer.h"



unsigned int line_number;

int Command(Compiler c, int oper)
{
	if(oper == OPERATOR_SEMI_COLON)
	{
		c->reg_num = 0;
	}
}

Identifier Command_Operation(Compiler c, Identifier A, int oper, Identifier B)
{
	switch(oper)
	{
		case	OPERATOR_SUB:
		case	OPERATOR_ADD:
		case	OPERATOR_MUL:
		case	OPERATOR_DIV:
		case	OPERATOR_MOD:
		case	OPERATOR_GTH:
		case	OPERATOR_LTH:
		case	OPERATOR_GTE:
		case	OPERATOR_LTE:
		case	OPERATOR_CMP:
		case	OPERATOR_NEQ:
		case	OPERATOR_OR: 
		case	OPERATOR_EQU: 
		case	OPERATOR_AND:
			{
				char buf1[64], buf2[64], buf3[64];
				Identifier res = Identifier_NewRegister(c->reg_num++);
				Identifier_to_str(A, buf1, 64);
				Identifier_to_str(B, buf2, 64);
				Identifier_to_str(res, buf3, 64);
				Identifier_Destroy(A);
				Identifier_Destroy(B);
				LOG_INFO_NL("%s %s %s %s", token_to_str(oper), buf1, buf2, buf3);
				return res;
			}
			break;
	}	
	return NULL;
}

int main(int argc, char *argv[])
{
	int yv;
	void *pParser;
	Compiler c;

	c = (Compiler)Malloc(sizeof(struct _Compiler));
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

	memset(c, 0, sizeof(Compiler));
	pParser = (void*)ParseAlloc(Malloc);
	while((yv=yylex()) != 0)
	{
		c->line_number = line_number;
		Parse(pParser, yv, yylval, c);
		if(c->error_flag == 1)
		{
			fprintf(stderr, "stopping parsing due to syntax error\n");
			break;
		}
	}	
	Parse(pParser, 0, yylval, c);
	ParseFree(pParser, Free);
	Free(c);
	fclose(yyin);
	yylex_destroy();
	return 0;
}
