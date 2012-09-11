#include "string.h"
#include "executable.h"

VariableList VariableList_Create(void)
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
		if(!IS_NULL(vl->variable_name) && strlen(vl->variable_name) != 0)
		{
			Free(vl->variable_name);
			Identifier_Destroy(vl->id);
		}
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
		//LOG_ERROR("Variable('%s') not found", variable_name);
		return NULL;
	}

	flag = 1;
	do
	{
		int tmp = strcasecmp(tmp_vl->variable_name, variable_name);
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
	//LOG_ERROR("Variable('%s') not found", variable_name);
	return NULL;
}


STATUS VariableList_AddVariable(VariableList vl, const char *variable_name, Identifier id)
{
	VariableList new_var, v, tmp_v;

	if(IS_NULL(id))
	{
		LOG_ERROR("trying to add NULL variable");
		return STATUS_FAILURE;
	}

	new_var = VariableList_Create();
	if(!IS_NULL(new_var))
	{
		new_var->id = Identifier_Clone(id);
		if(IS_NULL(new_var->id))
		{
			LOG_ERROR("Identifier_Clone() failed");
			return STATUS_FAILURE;
		}
		new_var->variable_name = (char*)Malloc(strlen(variable_name)+1);
		memcpy(new_var->variable_name, variable_name, strlen(variable_name));
	}
	else
	{
		LOG_ERROR("VariableList_Create() failed");
		return STATUS_FAILURE;
	}
	if(vl->next == NULL)
	{
		vl->next = new_var;
		return STATUS_SUCCESS;
	}
	else
	{
		tmp_v = vl;
		v = vl->next;
		while(!IS_NULL(v))
		{
			int tmp = strcasecmp(v->variable_name, variable_name);
			if(0 == tmp)
			{
				VariableList_Destroy(new_var);
				LOG_ERROR("variable with name '%s' already declared", variable_name);
				return STATUS_FAILURE;
			}
			else if(tmp > 0)
			{
				new_var->next = tmp_v->next;
				tmp_v->next = new_var;
				return STATUS_SUCCESS;
			}
			tmp_v = v;
			v = v->next;
		}
		if(IS_NULL(v))
		{
			tmp_v->next = new_var;
			return STATUS_SUCCESS;
		}
	}
	return STATUS_FAILURE;
}
