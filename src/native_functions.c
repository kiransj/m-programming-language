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

Identifier Function_TypeOf(Identifier *args, int num_args)
{
	if(args[0]->u.number != 1)
	{
		LOG_ERROR("typeof(variable) usage");
		return Identifier_NewInteger(0);
	}
	return (Identifier_NewString(IdentifierType_to_str(args[1]->type)));
}

#ifdef ENABLE_KEY_VALUE
typedef struct _KeyValue
{
	VariableList vl;
	VariableList cur_ptr;
}*KeyValue;

void KeyValue_delete(void *ptr)
{
	KeyValue keyValue = (KeyValue)ptr;
	VariableList_Destroy(keyValue->vl);
	memset(keyValue, 0, sizeof(struct _KeyValue));
	Free(keyValue);
	LOG_ERROR("Deleting the keyValue");
}

Identifier Function_KeyValue(Identifier *args, int num_args)
{
	Object 		obj = NULL;
	Identifier 	i = NULL;
	KeyValue	keyValue;

	keyValue = (KeyValue)Malloc(sizeof(struct _KeyValue));
	if(IS_NULL(keyValue))
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(struct _KeyValue));
		return Identifier_NewInteger(0);
	}
	keyValue->vl= VariableList_Create("\0");
	if(!IS_NULL(keyValue->vl))
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
	KeyValue keyValue;
	if(args[0]->u.number != 3 || args[1]->type != IDENTIFIER_TYPE_OBJECT || args[2]->type != IDENTIFIER_TYPE_STRING)
	{
		LOG_ERROR("KeyValueAdd(object, key, value) usage");
		return Identifier_NewInteger(0);
	}
	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		if(STATUS_SUCCESS == VariableList_AddVariable(keyValue->vl, args[2]->u.str, args[3]))
		{
			return Identifier_NewInteger(1);
		}
	}
	return Identifier_NewInteger(0);
}

Identifier Function_KeyValueGet(Identifier *args, int num_args)
{
	Identifier i = NULL;
	KeyValue keyValue;
	if(args[0]->u.number != 2 || args[1]->type != IDENTIFIER_TYPE_OBJECT || args[2]->type != IDENTIFIER_TYPE_STRING)
	{
		LOG_ERROR("KeyValueAdd(object, key, value) usage");
		return Identifier_NewInteger(0);
	}
	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		i = VariableList_FindVariable(keyValue->vl, args[2]->u.str);
		if(IS_NULL(i))
		{
			return Identifier_NewInteger(0);
		}
	}
	return Identifier_Clone(i);
}


Identifier Function_KeyValueIterator(Identifier *args, int num_args)
{
	KeyValue keyValue;
	if(args[0]->u.number != 1 || args[1]->type != IDENTIFIER_TYPE_OBJECT)
	{
		LOG_ERROR("KeyValueIterator(object) usage");
		return Identifier_NewInteger(0);
	}

	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		keyValue->cur_ptr = keyValue->vl;
		return Identifier_NewInteger(1);
	}
	return Identifier_NewInteger(0);
}
Identifier Function_KeyValueNext(Identifier *args, int num_args)
{
	KeyValue keyValue;
	if(args[0]->u.number != 1 || args[1]->type != IDENTIFIER_TYPE_OBJECT)
	{
		LOG_ERROR("KeyValueNext(object) usage");
		return Identifier_NewInteger(0);
	}

	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		if(IS_NULL(keyValue->cur_ptr))
		{
			LOG_ERROR("First call KeyValueIterator() and then call KeyValueNext");
			return Identifier_NewInteger(0);
		}
		keyValue->cur_ptr = keyValue->cur_ptr->next;
		return Identifier_NewInteger((IS_NULL(keyValue->cur_ptr) ? 0 : 1));
	}
	return Identifier_NewInteger(0);
}
Identifier Function_KeyValueGetKey(Identifier *args, int num_args)
{
	KeyValue keyValue;
	if(args[0]->u.number != 1 || args[1]->type != IDENTIFIER_TYPE_OBJECT)
	{
		LOG_ERROR("KeyValueGetKey(object) usage");
		return Identifier_NewInteger(0);
	}

	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		if(IS_NULL(keyValue->cur_ptr) || IS_NULL(keyValue->cur_ptr->variable_name))
		{
			LOG_ERROR("First call KeyValueNext() and then KeyValueGetName()");
			return Identifier_NewInteger(0);
		}
		return Identifier_NewString(keyValue->cur_ptr->variable_name);
	}
	return Identifier_NewInteger(0);
}

Identifier Function_KeyValueGetValue(Identifier *args, int num_args)
{
	KeyValue keyValue;
	if(args[0]->u.number != 1 || args[1]->type != IDENTIFIER_TYPE_OBJECT)
	{
		LOG_ERROR("KeyValueGetValue(object) usage");
		return Identifier_NewInteger(0);
	}

	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		if(IS_NULL(keyValue->cur_ptr) || IS_NULL(keyValue->cur_ptr->variable_name))
		{
			LOG_ERROR("First call KeyValueNext() and then KeyValueGetName()");
			return Identifier_NewInteger(0);
		}
		return Identifier_Clone(keyValue->cur_ptr->id);
	}
	return Identifier_NewInteger(0);
}
#endif


void Register_Native_Functions(Executable exe)
{
	Executable_AddNativeFunction(exe, "output", Function_Output);
	Executable_AddNativeFunction(exe, "max", Function_Max);
	Executable_AddNativeFunction(exe, "typeof", Function_TypeOf);
#ifdef ENABLE_KEY_VALUE
	Executable_AddNativeFunction(exe, "KeyValue", Function_KeyValue);
	Executable_AddNativeFunction(exe, "KeyValueAdd", Function_KeyValueAdd);
	Executable_AddNativeFunction(exe, "KeyValueGet", Function_KeyValueGet);
	Executable_AddNativeFunction(exe, "KeyValueIterator", Function_KeyValueIterator);
	Executable_AddNativeFunction(exe, "KeyValueNext", Function_KeyValueNext);
	Executable_AddNativeFunction(exe, "KeyValueGetKey", Function_KeyValueGetKey);
	Executable_AddNativeFunction(exe, "KeyValueGetValue", Function_KeyValueGetValue);
#endif

}

void UnRegister_Native_Functions(Executable exe)
{
	
}
