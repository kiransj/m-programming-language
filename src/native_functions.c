#include "util.h"
#include "executable.h"


Identifier Function_Output(Identifier *args, int num_args)
{
	int i;
	for(i = 1; i <= num_args; i++)
	{
		switch(args[i]->type)
		{
			case IDENTIFIER_TYPE_NUMBER:
					printf("%d", args[i]->u.number);
					break;
			case IDENTIFIER_TYPE_FLOAT:
					printf("%lf", args[i]->u.real);
					break;				
			case IDENTIFIER_TYPE_STRING:
					printf("%s", args[i]->u.str);
					break;
			default:
					printf("unknown id type '%d'", args[i]->type);
					abort();
		}
	}
	printf("\n");
	return NULL;
}

Identifier Function_Max(Identifier *args, int num_args)
{
	int i, max = 0;
	for(i = 1; i <= num_args; i++)
	{
		switch(args[i]->type)
		{
			case IDENTIFIER_TYPE_NUMBER:
					if(max < args[i]->u.number)
					{
						max = args[i]->u.number;
					}
					break;

			case IDENTIFIER_TYPE_FLOAT:
			case IDENTIFIER_TYPE_STRING:					
					LOG_ERROR("invalid argument to Max() function of type string");
					break;
			default:
					printf("unknown id type '%d'", args[i]->type);
					abort();
		}
	}
	return Identifier_NewInteger(max);
}


VariableList keyValue;

Identifier Function_KeyValueInit(Identifier *args, int num_args)
{
	keyValue = VariableList_Create("\0");
	return NULL;
}

Identifier Function_KeyValueSet(Identifier *args, int num_args)
{
	Identifier t;

	if(IS_NULL(keyValue))
	{
		LOG_ERROR("first call KeyValueInit()");
		return NULL;
	}
	if((args[0]->u.number != 2) || (args[1]->type != IDENTIFIER_TYPE_STRING))
	{
		LOG_ERROR("KeyValueSet(string=key, id=value)");
		return NULL;
	}
	t = VariableList_FindVariable(keyValue,args[1]->u.str);
	if(IS_NULL(t))
	{
		if(STATUS_SUCCESS == VariableList_AddVariable(keyValue,args[1]->u.str,args[2]))
			return Identifier_NewInteger(1);
		LOG_ERROR("KeyValueSet(%s) failed", args[1]->u.str);
	}
	else
	{
		Identifier_Copy(args[2],t);
		return Identifier_NewInteger(1);
	}
	return NULL;
}

Identifier Function_KeyValueGet(Identifier *args, int num_args)
{
	Identifier t;
	if(IS_NULL(keyValue))
	{
		LOG_ERROR("first call KeyValueInit()");
		return NULL;
	}
	if((args[0]->u.number != 1) || (args[1]->type != IDENTIFIER_TYPE_STRING))
	{
		LOG_ERROR("KeyValueGet(string=key)");
		return NULL;
	}
	t = VariableList_FindVariable(keyValue,args[1]->u.str);
	if(IS_NULL(t))
	{
		LOG_ERROR("KeyValueGet(%s) failed", args[1]->u.str);
		return NULL;
	}

	return Identifier_Clone(t);
}



void Register_Native_Functions(Executable exe)
{
	Executable_AddNativeFunction(exe, "output", Function_Output);
	Executable_AddNativeFunction(exe, "max", Function_Max);
	Executable_AddNativeFunction(exe, "KeyValueInit", Function_KeyValueInit);
	Executable_AddNativeFunction(exe, "KeyValueSet", Function_KeyValueSet);
	Executable_AddNativeFunction(exe, "KeyvalueGet", Function_KeyValueGet);
}

void UnRegister_Native_Functions(Executable exe)
{

}
