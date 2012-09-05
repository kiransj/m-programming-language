#ifndef __COMPILER_H_
#define __COMPILER_H_

#include "tokenizer.h"
typedef struct _Compiler 
{
	int line_number;
	int error_flag;
}Compiler;


Identifier Command(Compiler, Identifier A, int oper, Identifier B);

#endif
