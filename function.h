#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "tokenizer.h"
#include "util.h"
typedef enum
{
	FUNCTION_TYPE_UNDEF,
	FUNCTION_TYPE_NATIVE,
	FUNCTION_TYPE_LOCAL,
}FUNCTION_TYPE;

typedef Identifier (*NativeFunction)(Identifier *args, int num_args);
typedef struct _functionlist
{
	char 					*func_name;
	FUNCTION_TYPE			type;
	union
	{
		unsigned int 			address;
		NativeFunction			nFunc;	
	}u;
	struct _functionlist	*next;
}*FunctionList;

FunctionList FunctionList_Create(void);
void FunctionList_Destroy(FunctionList func);
FunctionList FunctionList_FindFunction(FunctionList fl, const char *func_name);
STATUS FunctionList_AddLocalFunction(FunctionList fl, const char *func_name, const unsigned int address);
STATUS FunctionList_AddNativeFunction(FunctionList fl,  const char *func_name, NativeFunction func);
#endif
