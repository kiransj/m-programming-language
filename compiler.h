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
Identifier Command_Operation(Compiler, Identifier A, int oper, Identifier B);

#endif
