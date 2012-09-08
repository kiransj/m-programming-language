#include "string.h"
#include "function.h"

FunctionList FunctionList_Create(void)
{
	FunctionList fl;
	fl = (FunctionList)Malloc(sizeof(struct _functionlist));
	memset(fl, 0, sizeof(struct _functionlist));
	return fl;
}

void FunctionList_Destroy(FunctionList func)
{
	while(func != NULL)
	{
		FunctionList tmp = func->next;
		if(!IS_NULL(func->func_name))
			Free(func->func_name);
		Free(func);
		func = tmp;
	}
}

STATUS FunctionList_AddLocalFunction(FunctionList fl, const char *func_name, const unsigned int address)
{
	FunctionList f ;
	FunctionList tmp = fl;

	if(IS_NULL(func_name) || (strlen(func_name) == 0))
	{
		LOG_ERROR("local function name is NULL");
		return STATUS_FAILURE;
	}
	while(tmp->next != NULL)
	{
		if(!IS_NULL(tmp->func_name) && strcasecmp(tmp->func_name, func_name) == 0)
		{
			LOG_ERROR("function name '%s' already exists", func_name);
			return STATUS_FAILURE;
		}
		tmp = tmp->next;
	}

	f = FunctionList_Create();
	if(!IS_NULL(f))
	{
		f->func_name = (char*)Malloc(strlen(func_name)+1);
		strcpy(f->func_name, func_name);
		f->type = FUNCTION_TYPE_LOCAL;
		f->u.address = address;
		tmp->next = f;
		return STATUS_SUCCESS;
	}
	LOG_ERROR("FunctionList_Create() failed");
	return STATUS_FAILURE;
}

STATUS FunctionList_AddNativeFunction(FunctionList fl, const char *func_name, NativeFunction func)
{
	FunctionList f ;
	FunctionList tmp = fl;

	if(IS_NULL(func_name) || (strlen(func_name) == 0))
	{
		LOG_ERROR("native function name is NULL");
		return STATUS_FAILURE;
	}
	while(tmp->next != NULL)
	{
		if(!IS_NULL(tmp->func_name) && strcasecmp(tmp->func_name, func_name) == 0)
		{
			LOG_ERROR("function name '%s' already exists", func_name);
			return STATUS_FAILURE;
		}
		tmp = tmp->next;
	}

	f = FunctionList_Create();
	if(!IS_NULL(f))
	{
		f->func_name = (char*)Malloc(strlen(func_name)+1);
		strcpy(f->func_name, func_name);
		f->type = FUNCTION_TYPE_NATIVE;
		f->u.nFunc = func;
		tmp->next = f;
		return STATUS_SUCCESS;
	}
	LOG_ERROR("FunctionList_Create() failed");
	return STATUS_FAILURE;
}

FunctionList FunctionList_FindFunction(FunctionList fl, const char *func_name)
{
	FunctionList tmp = fl->next;
	while(tmp != NULL)
	{
		if(strcasecmp(tmp->func_name, func_name) == 0)
		{
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}
