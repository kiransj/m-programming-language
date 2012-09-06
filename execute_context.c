#include <string.h>
#include "executable.h"

STATUS ExecutionContext_Execute(Executable exe)
{
	ExecutionContext ec;
	ec = exe->ec = ExecutionContext_Create(exe->first->next);
	
	while(!IS_NULL(ec->cur_ptr))
	{

	}
	return STATUS_SUCCESS;
}

ExecutionContext ExecutionContext_Create(ByteCode cur_ptr)
{
	ExecutionContext ec = NULL;
	if(!IS_NULL(cur_ptr))
	{
		ec = (ExecutionContext)Malloc(sizeof(struct _ExecutionContext));
		if(IS_NULL(ec))
		{
			LOG_ERROR("Malloc(%u) failed", sizeof(struct _ExecutionContext));
			return NULL;
		}
		memset(ec, 0, sizeof(struct _ExecutionContext));
		ec->cur_ptr = cur_ptr;

		ec->num_regs = 25;
		ec->regs = (Identifier)Malloc(sizeof(struct _Identifier) * ec->num_regs);
		if(IS_NULL(ec->regs))
		{
			LOG_ERROR("Malloc(%u) failed", sizeof(struct _Identifier) * ec->num_regs);
			Free(ec);
			return NULL;
		}
		memset(ec->regs, 0, sizeof(struct _Identifier) * ec->num_regs);

		ec->local_variables = VariableList_Create("\0");
		if(IS_NULL(ec->local_variables))
		{
			Free(ec->regs);
			Free(ec);
			LOG_ERROR("Variable_Create() failed");
			return NULL;
		}
	}
	else
	{
		LOG_ERROR("Trying to create a ExecutionContext pointing to NULL");
	}
	return ec;
}

VariableList VariableList_Create(const char *variable_name)
{
	VariableList vl;
	vl = (VariableList)Malloc(sizeof(struct _variablelist));
	if(IS_NULL(vl))
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(struct _variablelist));
		return NULL;
	}
	memset(vl, 0, sizeof(struct _variablelist));
	return vl;
}
void VariableList_Destroy(VariableList vl)
{
	VariableList tmp_vl;
	
	while(!IS_NULL(vl))
	{
		tmp_vl = vl->next;
		if(strlen(vl->variable_name) != 0) Free(vl->variable_name);
		Identifier_Destroy(vl->id);
		Free(vl);
		vl = tmp_vl;
	}	
}
Identifier  VariableList_FindVariable(VariableList vl, const char *variable_name)
{
	int flag;
	VariableList tmp_vl = vl->next, tmp1_vl;

	if(strlen(variable_name) < 1)
	{
		LOG_ERROR("variable_name is of size 0");
		return NULL;
	}

	if(IS_NULL(tmp_vl))
	{
		tmp_vl = VariableList_Create(variable_name);
		if(!IS_NULL(tmp_vl))
		{
			tmp_vl->id = Identifier_Create();
			tmp_vl->next = NULL;
			tmp_vl->variable_name = (char*)Malloc(strlen(variable_name)+1);
			memcpy(tmp_vl->variable_name, variable_name, strlen(variable_name)+1);
			vl->next = tmp_vl;
		}
		else
		{
			LOG_ERROR("VariableList_Create() Failed");
			return NULL;
		}
	}

	flag = 1;
	do
	{
		int tmp = strcmp(tmp_vl->variable_name, variable_name);
		if(tmp > 0)
		{
			flag = 2;
			break;
		}
		else if(tmp == 0)
		{
			flag = 0;
			break;
		}
		tmp1_vl = tmp_vl;
		tmp_vl = tmp_vl->next;
	}
	while(!IS_NULL(tmp_vl));

	if(flag == 0)
	{
		return tmp_vl->id;
	}
	else
	{
		tmp_vl = VariableList_Create(variable_name);
		if(!IS_NULL(tmp_vl))
		{
			tmp_vl->id = Identifier_Create();
			tmp_vl->next = NULL;
			tmp_vl->variable_name = (char*)Malloc(strlen(variable_name)+1);
			memcpy(tmp_vl->variable_name, variable_name, strlen(variable_name)+1);
			if(flag == 1)
			{
				tmp1_vl->next = tmp_vl;
			}
			else if(flag == 2)
			{
				tmp_vl->next = tmp1_vl->next;
				tmp1_vl->next = tmp_vl;
			}
			return tmp_vl->id;
		}
		else
		{
			LOG_ERROR("VariableList_Create() Failed");
			return NULL;
		}
	}
	return NULL;
}
