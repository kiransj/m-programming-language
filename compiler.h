#ifndef __COMPILER_H_
#define __COMPILER_H_

#include "tokenizer.h"
typedef struct _Compiler 
{
	int line_number;
	int error_flag;
	int reg_num;
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
}CompilerCmd;

void Command(Compiler, CompilerCmd oper);
void Command_FunctionArg(Compiler C, Identifier A, int pos);
Identifier Command_function_call(Compiler c, Identifier A, int num_args);
Identifier Command_Operation(Compiler, Identifier A, CompilerCmd oper, Identifier B);

#endif
