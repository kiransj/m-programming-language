#ifndef _EXECUTABLE_H_
#define _EXECUTABLE_H_

#include "compiler.h"
#include "tokenizer.h"
#include "util.h"

struct _ByteCode;
typedef struct _ByteCode *ByteCode;
struct _ByteCode
{
	CompilerCmd cmd;
	Identifier A, B, C;
	union
	{
		int label_number;
		int num_arguments;
		int	argument_pos;
	}u;
	ByteCode next;
};

typedef struct _variablelist
{
	char *variable_name;
	Identifier id;
	struct _variablelist *next;
}*VariableList;


VariableList VariableList_Create(const char *var_name);
void 		 VariableList_Destroy(VariableList);
Identifier   VariableList_FindVariable(VariableList, const char *variable_name);

typedef struct _ExecutionContext
{
	unsigned int num_regs;
	Identifier	 *regs;

	unsigned int num_args;
	Identifier	 *args;

	VariableList	local_variables;	

	ByteCode	 	cur_ptr;
}*ExecutionContext;

void ExecutionContext_Destroy(ExecutionContext ec);
ExecutionContext ExecutionContext_Create(ByteCode cur_ptr);

typedef struct _Executable
{
	ByteCode 	first, last;
	unsigned int label_size, label_index;
	unsigned int *label_list;

	/*Initialized during RunTime*/
	ExecutionContext ec;
}*Executable;

Executable Executable_Create(void);
void Executable_Destroy(Executable exe);
STATUS Executable_AddCmd(Executable exe, CompilerCmd cmd, Identifier A, Identifier B, Identifier C, int number);


STATUS ExecutionContext_Execute(Executable exe);
#endif
