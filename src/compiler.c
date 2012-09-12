#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "executable.h"
#include "compiler.h"
#include "util.h"


unsigned int line_number = 1;

/*Comment the below code to print the byte code*/
#ifndef PRINT_BYTE_CODE 

#ifdef LOG_INFO_NL
#undef LOG_INFO_NL
#define LOG_INFO_NL(format, args...)
#endif

#endif

const char* token_to_str(int token)
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
		c->reg_num = 1;
	}
}

int Command_LoopStmt(Compiler c, CompilerCmd cmd, Identifier A)
{
	int return_value=  0;
	char buf1[64];
	Executable exe = (Executable)c->priv_data;

	exe->line_number = c->line_number;
	if(STMT_START_WHILE == cmd)
	{
		int label_num = c->label_number++;
		c->label_stack[++c->label_top] = label_num;
		LOG_INFO_NL("LABEL_%d:", label_num);
		Executable_AddCmd(exe, LABEL, NULL, NULL, NULL, label_num);
		return_value = c->label_number++;
	}
	else if(STMT_WHILE_COND == cmd)
	{
		int label_number = c->label_stack[c->label_top]+1;
		Identifier_to_str(A, buf1, 64);
		LOG_INFO_NL("JZ %s, %d", buf1, label_number);
		Executable_AddCmd(exe, JZ, A, NULL, NULL, label_number);
		return_value= label_number;
	}
	else if(STMT_END_WHILE == cmd)
	{
		int label_number = c->label_stack[c->label_top--];
		LOG_INFO_NL("JUMP %d", label_number);
		LOG_INFO_NL("LABEL_%d:", label_number+1);

		Executable_AddCmd(exe, JUMP, NULL, NULL, NULL, label_number);
		Executable_AddCmd(exe, LABEL, NULL, NULL, NULL, label_number+1);
	}
	if(!IS_NULL(A))
	{
		Identifier_Destroy(A);
	}
	return return_value;
}

int Command_ConditionStmt(Compiler c, CompilerCmd cmd, Identifier A)
{
	char buf1[64];
	Executable exe = (Executable)c->priv_data;

	exe->line_number = c->line_number;
	if(STMT_IF == cmd)
	{
		int label_num = c->label_number++;
		c->label_stack[++c->label_top] = label_num;
		Identifier_to_str(A, buf1, 64);
		LOG_INFO_NL("JZ %s, %d", buf1, label_num);
		Executable_AddCmd(exe, JZ, A, NULL, NULL, label_num);

		Identifier_Destroy(A);
		return label_num;
	}
	else if(STMT_ENDIF == cmd)
	{
		int label_number = c->label_stack[c->label_top--];
		LOG_INFO_NL("LABEL_%d:", label_number);
		Executable_AddCmd(exe, LABEL, NULL, NULL, NULL, label_number);
	}
	return 0;
}

void Command_FunctionArg(Compiler c, Identifier A)
{
	char buf1[64];
	Executable exe = (Executable)c->priv_data;

	exe->line_number = c->line_number;
	Identifier_to_str(A, buf1, 64);
	LOG_INFO_NL("PUSH %s", buf1);
	Executable_AddCmd(exe, PUSH, A, NULL, NULL, 0);
	Identifier_Destroy(A);
	return ;
}


void Command_VariableDecl(Compiler c, Identifier A, Identifier B)
{
	char buf1[64], buf2[64];
	Executable exe = (Executable)c->priv_data;

	exe->line_number = c->line_number;
	Identifier_to_str(A, buf1, 64);
	if(IS_NULL(B))
	{
		B =  Identifier_NewInteger(0);
	}
	Identifier_to_str(B, buf2, 64);
	LOG_INFO_NL("VAR %s, %s", buf1, buf2);
	Executable_AddCmd(exe, VAR, A, B, NULL, 0);
	Identifier_Destroy(A);
	Identifier_Destroy(B);
	return ;
}
Identifier Command_function_call(Compiler c, Identifier A, int num_args)
{
	char buf2[64];
	Executable exe = (Executable)c->priv_data;
	Identifier res = NULL;
	exe->line_number = c->line_number;
	res = Identifier_NewRegister(c->reg_num++);
	if(IS_NULL(res))
	{
		LOG_ERROR("Identifier_NewRegister() failed");
		c->error_flag = 1;
		return NULL;
	}
	if(c->max_num_reg < c->reg_num)
	{
		c->max_num_reg = c->reg_num;
	}	
	Identifier_to_str(res, buf2, 64);
	LOG_INFO_NL("CALL %s, %d, %s", A->u.variable_name, num_args, buf2);
	Executable_AddCmd(exe, CALL, A, NULL, res, num_args);
	Identifier_Destroy(A);
	return res;
}

void Command_NewFunction(Compiler c, Identifier func_name)
{
	Executable exe = (Executable)c->priv_data;
	exe->line_number = c->line_number;
	LOG_INFO_NL("FUNCTION %s", func_name->u.str);
	Executable_AddCmd(exe, FUNCTION, func_name, NULL, NULL, 0);
	Identifier_Destroy(func_name);
	return;
}

void Command_EndFunction(Compiler c)
{
	Executable exe = (Executable)c->priv_data;
	Identifier ret = Identifier_NewInteger(0);
	exe->line_number = c->line_number;
	LOG_INFO_NL("ENDFUNCTION ");
	Executable_AddCmd(exe, RETURN, ret, NULL, NULL, 0);
	Identifier_Destroy(ret);
	return;
}

void Command_ReturnStmt(Compiler c, Identifier ret_value)
{
	Executable exe = (Executable)c->priv_data;
	char buf1[64];
	exe->line_number = c->line_number;
	Identifier_to_str(ret_value, buf1, 64);
	LOG_INFO_NL("RETURN %s", buf1);
	Executable_AddCmd(exe, RETURN, ret_value, NULL, NULL, 0);
	Identifier_Destroy(ret_value);
}
Identifier Command_Operation(Compiler c, Identifier A, CompilerCmd oper, Identifier B)
{
	Executable exe = (Executable)c->priv_data;
	exe->line_number = c->line_number;
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
				if(c->max_num_reg < c->reg_num)
				{
					c->max_num_reg = c->reg_num;
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

void* ParseAlloc(void *(*)(size_t));
void* ParseFree(void*, void (*)(void *));
void Parse(void*, int, Identifier, Compiler);

STATUS Compile(Executable exe, const char *filename)
{
	int yv;
	void *pParser;
	STATUS ret_value = STATUS_FAILURE;
	Compiler c;

	if(IS_NULL(exe))
	{
		LOG_ERROR("exe variable is NULL");
		return ret_value;
	}


	c = (Compiler)Malloc(sizeof(struct _Compiler));
	memset(c, 0, sizeof(Compiler));
	c->label_number = 1;
	c->label_size = 100;
	c->label_top = -1;
	c->label_stack = (int *)Malloc(sizeof(int) * c->label_size);

	c->reg_num = 1;
	c->priv_data = (void*)exe;
	Register_Native_Functions(exe);

	yyin = fopen(filename, "r");
	if(NULL == yyin)
	{
		Free(c);
		printf("unable to open file\n");
		return ret_value;
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
	Parse(pParser, 0, yylval, c);
	ParseFree(pParser, Free);

	exe->max_num_reg = c->max_num_reg;
	LOG_INFO_NL("max number of regs required : %d", c->max_num_reg);

	if(c->error_flag == 0)
	{
		ret_value = STATUS_SUCCESS;
	}
	Free(c);	
	fclose(yyin);
	yylex_destroy();
	return ret_value;
}
