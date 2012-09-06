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

struct _Executable;
typedef struct _Executable *Executable;
struct _Executable
{
	ByteCode 	first, last;			

	unsigned int num_regs;
	Identifier	 *regs;
	unsigned int num_args;
	Identifier	 *args;

	unsigned int label_size, label_index;
	unsigned int *label_list;
};

Executable Executable_Create(void);
void Executable_Destroy(Executable exe);
STATUS Executable_AddCmd(Executable exe, CompilerCmd cmd, Identifier A, Identifier B, Identifier C, int number);
#endif
