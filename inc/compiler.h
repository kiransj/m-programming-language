#ifndef __COMPILER_H_
#define __COMPILER_H_

#include "identifier.h"

typedef struct _Compiler 
{
	int line_number;
	int error_flag;
	int reg_num, max_num_reg;	
	void *priv_data;


	int label_number;
	int *label_stack;
	int label_top, label_size;
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
	NOT,
	EQU,

	JUMP,
	JZ,
	CALL,
	LABEL,
	PUSH,
	VAR,

	FUNCTION,
	ENDFUNCTION,
	RETURN,

	STMT_END,
	STMT_IF,
	STMT_ELSE,
	STMT_ELIF_KEYWORD,
	STMT_ELIF_CONDITION,
	STMT_ENDIF,
	STMT_WHILE_COND,
	STMT_START_WHILE,
	STMT_END_WHILE,
}CompilerCmd;

void Command(Compiler, CompilerCmd oper);
void Command_FunctionArg(Compiler C, Identifier A);
void Command_VariableDecl(Compiler, Identifier, Identifier);

int  Command_ConditionStmt(Compiler c, CompilerCmd cmd, Identifier A);
int Command_LoopStmt(Compiler c, CompilerCmd cmd, Identifier A);

Identifier Command_function_call(Compiler c, Identifier A, int num_args);
Identifier Command_Operation(Compiler, Identifier A, CompilerCmd oper, Identifier B);

void Command_NewFunction(Compiler c, Identifier func_name);
void Command_EndFunction(Compiler c);


#endif
