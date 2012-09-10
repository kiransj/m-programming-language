#include "util.h"
#include "executable.h"
#include "string.h"

/* 
 * To avoid memory leaks please follow the following rules.
 * Every return value should be created here or cloned here
 */

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


void KeyValue_delete(void *ptr)
{
	VariableList keyValue = (VariableList)ptr;
	VariableList_Destroy(keyValue);
	LOG_ERROR("Deleting the keyValue");
}

Identifier Function_KeyValueInit(Identifier *args, int num_args)
{
	Object obj = NULL;
	Identifier i = NULL;
	VariableList keyValue;
	keyValue = VariableList_Create("\0");
	if(!IS_NULL(keyValue))
	{
		obj = (Object)Malloc(sizeof(struct _Object));
		memset(obj, 0, sizeof(struct _Object));
		obj->priv_data = (void*)keyValue;
		obj->obj_delete = KeyValue_delete;
		i = Identifier_NewObject(obj); 
	}
	return i;
}

Identifier Function_KeyValueAdd(Identifier *args, int num_args)
{
	VariableList vl;
	if(args[0]->u.number != 3 || args[1]->type != IDENTIFIER_TYPE_OBJECT || args[2]->type != IDENTIFIER_TYPE_STRING)
	{
		LOG_ERROR("KeyValueAdd(object, key, value) usage");
		return Identifier_NewInteger(0);
	}
	vl = (VariableList)args[1]->u.obj->priv_data;
	if(!IS_NULL(vl))
	{
		if(STATUS_SUCCESS == VariableList_AddVariable(vl, args[2]->u.str, args[3]))
		{
			return Identifier_NewInteger(1);
		}
	}
	return Identifier_NewInteger(0);
}

Identifier Function_KeyValueGet(Identifier *args, int num_args)
{
	Identifier i = NULL;
	VariableList vl;
	if(args[0]->u.number != 2 || args[1]->type != IDENTIFIER_TYPE_OBJECT || args[2]->type != IDENTIFIER_TYPE_STRING)
	{
		LOG_ERROR("KeyValueAdd(object, key, value) usage");
		return Identifier_NewInteger(0);
	}
	vl = (VariableList)args[1]->u.obj->priv_data;
	if(!IS_NULL(vl))
	{
		i = VariableList_FindVariable(vl, args[2]->u.str);
		if(IS_NULL(i))
		{
			return Identifier_NewInteger(0);
		}
	}
	return Identifier_Clone(i);
}

Identifier Function_TypeOf(Identifier *args, int num_args)
{
	if(args[0]->u.number != 1)
	{
		LOG_ERROR("typeof(variable) usage");
		return Identifier_NewInteger(0);
	}
	return (Identifier_NewString(IdentifierType_to_str(args[1]->type)));
}
void Register_Native_Functions(Executable exe)
{
	Executable_AddNativeFunction(exe, "output", Function_Output);
	Executable_AddNativeFunction(exe, "max", Function_Max);
	Executable_AddNativeFunction(exe, "KeyValueInit", Function_KeyValueInit);
	Executable_AddNativeFunction(exe, "KeyValueAdd", Function_KeyValueAdd);
	Executable_AddNativeFunction(exe, "KeyValueGet", Function_KeyValueGet);
	Executable_AddNativeFunction(exe, "typeof", Function_TypeOf);
}

void UnRegister_Native_Functions(Executable exe)
{
	
}
