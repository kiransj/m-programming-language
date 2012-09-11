#ifndef _EXECUTABLE_H_
#define _EXECUTABLE_H_

#include "M.h"
#include "compiler.h"
#include "tokenizer.h"
#include "function.h"
#include "variable_list.h"
#include "util.h"

struct _ByteCode;
typedef struct _ByteCode *ByteCode;
struct _ByteCode
{
	int line_number;
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

struct _ExecutionContext
{
	char			*func_name;

	unsigned int num_regs;
	Identifier	 *regs;

	unsigned int num_args;
	Identifier	 *args;

	VariableList	local_variables;

	ByteCode	 	cur_ptr;	
};

ExecutionContext ExecutionContext_Create(ByteCode cur_ptr);
void ExecutionContext_Destroy(ExecutionContext ec);

struct _Executable
{
	int				line_number;
	ByteCode 		first, last;

	/* Number of registered required*/
	int max_num_reg;
	
	Identifier ret_value;
	/*Data structure to save label and their address*/
	unsigned int 	label_size, label_index;
	unsigned int   *label_list;

	/* Stores function name and their address. Two types of functions are possible.
	 * 1. Local function.  (function in the script)	
	 * 2. Native function. (registered C function) */
	FunctionList	func_list;

	/* This stack is to pass arguments to called functions and also stores the 
	 * return value*/
	IdentifierStack		is;

	/*Initialized during RunTime*/
	ExecutionContext 	ec;
	int 				ec_size, ec_top;
	ExecutionContext 	*ec_list;
	int					error_flag;
};

void Executable_Destroy(Executable exe);
STATUS Executable_GrowExecutionContext(Executable exe);
STATUS Executable_AddCmd(Executable exe, CompilerCmd cmd, Identifier A, Identifier B, Identifier C, int number);


void RaiseException(Executable exe, const char *format, ...);
#endif
