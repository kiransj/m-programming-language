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


void map_delete(void *ptr)
{
	Map m = (Map)ptr;
	VariableList_Destroy(m->list);
	Free(m);
}

Identifier Map_Create(void)
{
	Object o = (Object)Malloc(sizeof(struct _Object));
	if(!IS_NULL(o))
	{
		Map m = (Map)Malloc(sizeof(struct _map));
		m->list = VariableList_Create();
		o->priv_data = (void*)m;
		strcpy(o->type, "struct");
		o->obj_delete = map_delete;
		return Identifier_NewObject(o);
	}
	else
	{
		LOG_ERROR("Malloc() failed");
	}
	return NULL;
}

Identifier Map_FindElement(Identifier m, const char *element_name)
{
	Identifier i;
	Map map = (Map)m->u.obj->priv_data;

	i = VariableList_FindVariable(map->list,element_name);
	if(IS_NULL(i))
	{
		i = Identifier_NewInteger(0);
		if(STATUS_FAILURE == VariableList_AddVariable(map->list,element_name,i))
		{
			LOG_ERROR("VariableList_AddVariable() failed");
			Identifier_Destroy(i);
			return NULL;
		}
		Identifier_Destroy(i);
		i = VariableList_FindVariable(map->list, element_name);
	}
	return i;
}

STATUS Map_AddElement(Identifier Obj, const char *element_name, Identifier value)
{
	Identifier i = Map_FindElement(Obj, element_name);
	if(!IS_NULL(i))
	{
		Identifier_Copy(i, value);
		return STATUS_SUCCESS;
	}
	return STATUS_FAILURE;
}
STATUS Map_AddString(Identifier Obj, const char *element_name, const char *string)
{
	Identifier i = Map_FindElement(Obj, element_name);
	if(!IS_NULL(i))
	{
		Identifier_SetString(i, (char*)string);
		return STATUS_SUCCESS;
	}
	return STATUS_FAILURE;
}
STATUS Map_AddInt(Identifier Obj, const char *element_name, const int num)
{
	Identifier i = Map_FindElement(Obj, element_name);
	if(!IS_NULL(i))
	{
		Identifier_SetInt(i, num);
		return STATUS_SUCCESS;
	}
	return STATUS_FAILURE;
}

void array_delete(void *ptr)
{
	Array a = (Array)ptr, tmp;
	while(!IS_NULL(a))
	{
		tmp = a->next;		
		Identifier_Destroy(a->index);
		Identifier_Destroy(a->value);
		Free(a);
		a = tmp;
	}
}
Identifier Array_Create(void)
{
	Object o = (Object)Malloc(sizeof(struct _Object));
	if(!IS_NULL(o))
	{
		Array a = (Array)Malloc(sizeof(struct _array));
		memset(a, 0, sizeof(struct _map));
		a->value = NULL;
		o->priv_data = (void*)a;
		strcpy(o->type, "array");
		o->obj_delete = array_delete;
		return Identifier_NewObject(o);
	}
	else
	{
		LOG_ERROR("Malloc() failed");
	}
	return NULL;
}

Identifier Array_FindIndex(Identifier obj, Identifier index)
{
	Array a = (Array)obj->u.obj->priv_data;
	while(!IS_NULL(a))
	{
		if(!IS_NULL(a->index) && (a->index->type == index->type))
		{
			switch(a->index->type)
			{
				case IDENTIFIER_TYPE_STRING:
						if(0 == strcmp(a->index->u.str, index->u.str))
						{
							return a->value;
						}
						break;
				case IDENTIFIER_TYPE_NUMBER:
						if(a->index->u.number == index->u.number)
						{
							return a->value;
						}
						break;
				default:
						{
							LOG_ERROR("unknown type %d in ArrayList", a->index->type);
							abort();
						}
			}
		}
		a = a->next;
	}
	return NULL;
}

STATUS Array_AddElement(Identifier Obj, Identifier index, Identifier value)
{
	Array a = (Array)Obj->u.obj->priv_data, tmp;

	if(index->type != IDENTIFIER_TYPE_STRING && index->type != IDENTIFIER_TYPE_NUMBER)
	{
		LOG_ERROR("invalid input type %d to ArrayAddElement()", index->type);
		abort();
	}

	while(!IS_NULL(a))
	{
		tmp = a;
		a = a->next;
	}
	tmp->next = (Array)Malloc(sizeof(struct _array));
	if(IS_NULL(tmp->next))
	{
		LOG_ERROR("Malloc() failed");
		return STATUS_FAILURE;
	}
	tmp->next->index = Identifier_Clone(index);
	tmp->next->value = (IS_NULL(value)) ? Identifier_NewInteger(0) : Identifier_Clone(value);
	return STATUS_SUCCESS;
}
