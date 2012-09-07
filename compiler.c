#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "executable.h"
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
		if(c->reg_num > c->max_num_reg)
			c->max_num_reg = c->reg_num;
		c->reg_num = 1;
	}
}

int Command_LoopStmt(Compiler c, CompilerCmd cmd, Identifier A, int label_number)
{
	int return_value=  0;
	char buf1[64];
	Executable exe = (Executable)c->priv_data;
	if(STMT_START_WHILE == cmd)
	{		
		int label_number = c->label_number++;
		LOG_INFO_NL("LABEL_%d:", label_number);
		Executable_AddCmd(exe, LABEL, NULL, NULL, NULL, label_number);
		return_value = c->label_number++;
	}
	else if(STMT_WHILE_COND == cmd)
	{		
		Identifier_to_str(A, buf1, 64);
		LOG_INFO_NL("JZ %s, %d", buf1, label_number);
		Executable_AddCmd(exe, JZ, A, NULL, NULL, label_number);
		return_value= label_number;
	}
	else if(STMT_END_WHILE == cmd)
	{
		LOG_INFO_NL("JUMP %d", label_number-1);		
		LOG_INFO_NL("LABEL_%d:", label_number);

		Executable_AddCmd(exe, JUMP, NULL, NULL, NULL, label_number-1);
		Executable_AddCmd(exe, LABEL, NULL, NULL, NULL, label_number);
	}
	if(!IS_NULL(A))
	{
		Identifier_Destroy(A);
	}
	return return_value;
}

int Command_ConditionStmt(Compiler c, CompilerCmd cmd, Identifier A, int label_number)
{
	char buf1[64];
	Executable exe = (Executable)c->priv_data;
	if(STMT_IF == cmd)
	{
		int label_number = c->label_number++;
		Identifier_to_str(A, buf1, 64);
		LOG_INFO_NL("JZ %s, %d", buf1,label_number);
		Executable_AddCmd(exe, JZ, A, NULL, NULL, label_number);

		Identifier_Destroy(A);
		return label_number;
	}
	else if(STMT_ENDIF == cmd)
	{
		LOG_INFO_NL("LABEL_%d:", label_number);
		Executable_AddCmd(exe, LABEL, NULL, NULL, NULL, label_number);
	}	
	return 0;
}

void Command_FunctionArg(Compiler c, Identifier A)
{
	char buf1[64];
	Executable exe = (Executable)c->priv_data;
	Identifier_to_str(A, buf1, 64);
	LOG_INFO_NL("PUSH %s", buf1);
	Executable_AddCmd(exe, PUSH, A, NULL, NULL, 0);
	Identifier_Destroy(A);
	return ;
}
Identifier Command_function_call(Compiler c, Identifier A, int num_args)
{
	char buf2[64];
	Executable exe = (Executable)c->priv_data;
	Identifier res = Identifier_NewRegister(c->reg_num++);
	if(IS_NULL(res))
	{
		LOG_ERROR("Identifier_NewRegister() failed");
		c->error_flag = 1;
		return NULL;
	}

	Identifier_to_str(res, buf2, 64);
	LOG_INFO_NL("CALL %s, %d, %s", A->u.variable_name, num_args, buf2);
	Executable_AddCmd(exe, CALL, A, NULL, NULL, num_args);
	Identifier_Destroy(A);
	return res;
}
Identifier Command_Operation(Compiler c, Identifier A, CompilerCmd oper, Identifier B)
{
	Executable exe = (Executable)c->priv_data;
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
				if(IS_NULL(res))
				{
					LOG_ERROR("Identifier_NewRegister() failed");
					c->error_flag = 1;
					return NULL;
				}			
				Identifier_to_str(A, buf1, 64);
				Identifier_to_str(B, buf2, 64);
				Identifier_to_str(res, buf3, 64);
				Executable_AddCmd(exe, oper, A, B, res, 0);				
				LOG_INFO_NL("%s %s, %s, %s", token_to_str(oper), buf1, buf2, buf3);

				Identifier_Destroy(A);
				Identifier_Destroy(B);
				return res;
			}
			break;
		default: 
			break;
	}
	return NULL;
}


Identifier Function_Printf(Identifier *args, int num_args)
{
	Identifier i = NULL;
	LOG_INFO("number of arguments passed : %d", num_args);
	return i;
}

void* ParseAlloc(void *(*)(size_t));
void* ParseFree(void*, void (*)(void *));
void Parse(void*, int, Identifier, Compiler);
int main(int argc, char *argv[])
{
	int yv;
	void *pParser;
	Compiler c;
	Executable exe;

	c = (Compiler)Malloc(sizeof(struct _Compiler));
	if(argc == 1)
	{
		LOG_INFO_NL("usage %s filename", argv[0]);
		return 1;
	}

	memset(c, 0, sizeof(Compiler));
	c->label_number = 1;
	c->reg_num = 1;
	exe = Executable_Create();
	if(IS_NULL(exe))
	{
		LOG_ERROR("Executable_Create() failed");
		Free(c);
		return 1;
	}
	Executable_AddNativeFunction(exe, "printf", Function_Printf);
	c->priv_data = (void*)exe;

	yyin = fopen(argv[1], "r");
	if(NULL == yyin)
	{
		printf("unable to open file\n");
		return 1;
	}

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
	LOG_INFO_NL("max number of regs required : %d", c->max_num_reg);
	Parse(pParser, 0, yylval, c);
	ParseFree(pParser, Free);
	ExecutionContext_Execute(exe);
	Free(c);
	Executable_Destroy(exe);
	fclose(yyin);
	yylex_destroy();
	return 0;
}
