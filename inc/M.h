#ifndef _M_H_
#define _M_H_

#include "variable_list.h"

struct _ExecutionContext;
typedef struct _ExecutionContext *ExecutionContext;

struct _Executable;
typedef struct _Executable *Executable;

Executable Executable_Create(void);;
void 	   Executable_Destroy(Executable exe);

STATUS Compile(Executable exe, const char *filename);
STATUS ExecutionContext_Execute(Executable exe, const char *func_name, Identifier Obj);
int ExecutionContext_GetReturnValue(Executable exe);
#endif
