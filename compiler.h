#ifndef __COMPILER_H_
#define __COMPILER_H_

#include "tokenizer.h"
typedef struct _Compiler 
{
	int line_number;
	int error_flag;
	int reg_num, max_num_reg;

	int label_number;
	int label_stack[1024], label_top;
}*Compiler;


typedef enum
{
	SUB,
	ADD,
	MUL,
	DIV,
	MOD,
	GTH,
	LTH,
	GTE,
	LTE,
	CMP,
	NEQ,
	OR, 
	AND,
	EQU,
	STMT_END,
	
	STMT_IF,
	STMT_ENDIF,
}CompilerCmd;

void Command(Compiler, CompilerCmd oper);
void Command_FunctionArg(Compiler C, Identifier A, int pos);
void Command_ConditionStmt(Compiler c, CompilerCmd cmd, Identifier A);

Identifier Command_function_call(Compiler c, Identifier A, int num_args);
Identifier Command_Operation(Compiler, Identifier A, CompilerCmd oper, Identifier B);

#endif
