#ifndef _EXECUTABLE_H_
#define _EXECUTABLE_H_

#include "compiler.h"
#include "tokenizer.h"
#include "function.h"
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

typedef struct _variablelist
{
	char *variable_name;
	Identifier id;
	struct _variablelist *next;
}*VariableList;


VariableList VariableList_Create(const char *var_name);
void 		 VariableList_Destroy(VariableList);
STATUS 		 VariableList_AddVariable(VariableList vl, const char *variable_name, Identifier v);
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

ExecutionContext ExecutionContext_Create(ByteCode cur_ptr);
void ExecutionContext_Destroy(ExecutionContext ec);

typedef struct _Executable
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
}*Executable;

Executable Executable_Create(void);
void Executable_Destroy(Executable exe);
STATUS Executable_GrowExecutionContext(Executable exe);
STATUS Executable_AddCmd(Executable exe, CompilerCmd cmd, Identifier A, Identifier B, Identifier C, int number);

STATUS Executable_AddNativeFunction(Executable exe, const char *func_name, NativeFunction nf);

STATUS ExecutionContext_Execute(Executable exe, const char *func_name);


Executable Compile(const char *filename);
void Register_Native_Functions(Executable exe);
void RaiseException(Executable exe, const char *format, ...);
#endif
