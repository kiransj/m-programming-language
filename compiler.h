#ifndef __COMPILER_H_
#define __COMPILER_H_

#include "tokenizer.h"
typedef struct _Compiler 
{
	int line_number;
	int error_flag;
	int reg_num;
}*Compiler;


int Command(Compiler,int oper);
void Command_Push(Compiler C, Identifier A);
Identifier Command_function_call(Compiler c, Identifier A, int num_args);
Identifier Command_Operation(Compiler, Identifier A, int oper, Identifier B);

enum
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
	PUSH,
	STMT_END,
};

#endif
