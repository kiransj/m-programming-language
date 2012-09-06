#ifndef __COMPILER_H_
#define __COMPILER_H_

#include "tokenizer.h"


typedef struct _Compiler 
{
	int line_number;
	int error_flag;
	int reg_num, max_num_reg;
	int label_number;
	void *priv_data;
}*Compiler;


typedef enum
{
	UNDEF,
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

	JUMP,
	JZ,
	CALL,
	LABEL,
	ARGUMENT,

	STMT_END,
	STMT_IF,
	STMT_ENDIF,
	STMT_WHILE_COND,
	STMT_START_WHILE,
	STMT_END_WHILE,
}CompilerCmd;

void Command(Compiler, CompilerCmd oper);
void Command_FunctionArg(Compiler C, Identifier A, int pos);
int  Command_ConditionStmt(Compiler c, CompilerCmd cmd, Identifier A, int label_number);
int Command_LoopStmt(Compiler c, CompilerCmd cmd, Identifier A, int label_number);

Identifier Command_function_call(Compiler c, Identifier A, int num_args);
Identifier Command_Operation(Compiler, Identifier A, CompilerCmd oper, Identifier B);

#endif
