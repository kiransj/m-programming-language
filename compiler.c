#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "compiler.h"
#include "util.h"


unsigned int line_number;

char* token_to_str(int token)
{
	switch(token)
	{
		case EQU:			return "MOV";
		case AND:			return "AND";
		case OR:			return "OR";
		case CMP:			return "CMP";
		case NEQ:			return "NEQ";
		case GTH:			return "GTH";								
		case GTE:			return "GTE";
		case LTH:			return "LTH";
		case LTE:			return "LTE";
		case ADD:			return "ADD";
		case SUB:			return "SUB";
		case DIV:			return "DIV";
		case MUL:			return "MUL";
		case MOD:			return "MOD";
		default:			return "NULL";
	}
}


void Command(Compiler c, CompilerCmd oper)
{
	if(oper == STMT_END)
	{
		c->reg_num = 0;
	}
}

void Command_FunctionArg(Compiler C, Identifier A, int pos)
{
	char buf1[64];
	Identifier_to_str(A, buf1, 64);
	LOG_INFO_NL("ARG[%d] %s", pos, buf1);
	Identifier_Destroy(A);
	return ;
}
Identifier Command_function_call(Compiler c, Identifier A, int num_args)
{
	char buf2[64];
	Identifier res = Identifier_NewRegister(c->reg_num++);
	Identifier_to_str(res, buf2, 64);
	LOG_INFO_NL("CALL %s %d %s", A->u.variable_name, num_args, buf2);
	Identifier_Destroy(A);
	return res;
}
Identifier Command_Operation(Compiler c, Identifier A, CompilerCmd oper, Identifier B)
{
	switch(oper)
	{
		case	SUB:
		case	ADD:
		case	MUL:
		case	DIV:
		case	MOD:
		case	GTH:
		case	LTH:
		case	GTE:
		case	LTE:
		case	CMP:
		case	NEQ:
		case	OR: 
		case	EQU: 
		case	AND:
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
	default: break;			
	}	
	return NULL;
}

void* ParseAlloc(void *(*)(size_t));
void* ParseFree(void*, void (*)(void *));
void Parse(void*, int, Identifier, Compiler);
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
