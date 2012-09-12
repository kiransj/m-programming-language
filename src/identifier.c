#include <string.h>
#include "identifier.h"
#include "util.h"
#include "compiler.h"

void Identifier_to_str(Identifier id, char * const buffer, const int size)
{
	if(IS_NULL(id))
	{
		snprintf(buffer, size, "NULL");
		return;
	}
	switch(id->type)
	{

		case IDENTIFIER_TYPE_NUMBER:	
			{
				snprintf(buffer, size, "int(%d)", id->u.number);
				break;
			}
		case IDENTIFIER_TYPE_FLOAT:		
			{
				snprintf(buffer, size, "float(%lf)", id->u.real);
				break;
			}
		case IDENTIFIER_TYPE_STRING:	
			{	
				snprintf(buffer, size, "string(%s)", id->u.str);
				break;
			}
		case IDENTIFIER_TYPE_ARGUMENT:	
			{
				snprintf(buffer, size, "arg(%d)", id->u.argument_number);
				break;
			}
		case IDENTIFIER_TYPE_VARIABLE:	
			{
				snprintf(buffer, size, "var(%s)", id->u.variable_name);
				break;
			}
		case IDENTIFIER_TYPE_REGISTER:	
			{
				snprintf(buffer, size, "reg(%d)", id->u.register_number);
				break;
			}
		case IDENTIFIER_TYPE_MAP:
			{
				snprintf(buffer, size, "map(%s->%s)", id->u.map->map_name, id->u.map->element_name);
				break;
			}
		default: 
			strcpy(buffer, "unknown"); 
			return;
	}
	return;
}
const char* IdentifierType_to_str(IdentifierType type)
{
	switch(type)
	{

		case IDENTIFIER_TYPE_NUMBER:	return "int";
		case IDENTIFIER_TYPE_FLOAT:		return "float";
		case IDENTIFIER_TYPE_STRING:	return "string";
		case IDENTIFIER_TYPE_ARGUMENT:	return "arg";
		case IDENTIFIER_TYPE_VARIABLE:	return "var";
		case IDENTIFIER_TYPE_REGISTER:	return "reg";
		case IDENTIFIER_TYPE_OBJECT:	return "object";
		case IDENTIFIER_TYPE_MAP:		return "map";
		default: break;
	}
	return "NULL";
}


Identifier Identifier_NewString(const char *str)
{
	Identifier i = Identifier_Create();
	if(!IS_NULL(i))
	{
		i->u.str = Malloc(strlen(str)+1);
		if(!IS_NULL(i->u.str))
		{
			i->type = IDENTIFIER_TYPE_STRING;
			strcpy(i->u.str, str);
		}
		else
		{
			Identifier_Destroy(i);
			i = NULL;
		}	
	}
	else
	{
		LOG_ERROR("Identifier_Create() failed");
	}
	return i;
}

Identifier Identifier_NewVariable(const char *variable_name)
{
	Identifier i = Identifier_Create();

	if(!IS_NULL(i))
	{
		i->u.variable_name= Malloc(strlen(variable_name)+1);
		if(!IS_NULL(i->u.variable_name))
		{
			i->type = IDENTIFIER_TYPE_VARIABLE;
			strcpy(i->u.variable_name, variable_name);
		}
		else
		{
			Identifier_Destroy(i);
			i = NULL;
		}
	}
	else
	{
		LOG_ERROR("Identifier_Create() failed");
	}

	return i;
}

Identifier Identifier_NewFloat(const double real)
{
	Identifier i = Identifier_Create();
	if(!IS_NULL(i))
	{
		i->type = IDENTIFIER_TYPE_FLOAT;
		i->u.real = real;
	}
	else
	{
		LOG_ERROR("Identifier_Create() failed");
	}
	return i;
}

Identifier Identifier_NewArgument(const int argument_number)
{
	Identifier i = Identifier_Create();
	if(!IS_NULL(i))
	{
		i->type = IDENTIFIER_TYPE_ARGUMENT;
		i->u.argument_number = argument_number;
	}
	else
	{
		LOG_ERROR("Identifier_Create() failed");
	}

	return i;
}
Identifier Identifier_NewInteger(const int number)
{
	Identifier i = Identifier_Create();
	if(!IS_NULL(i))
	{
		i->type = IDENTIFIER_TYPE_NUMBER;
		i->u.number = number;
	}
	else
	{
		LOG_ERROR("Identifier_Create() failed");
	}

	return i;
}

Identifier Identifier_NewRegister(const int number)
{
	Identifier i = Identifier_Create();
	if(!IS_NULL(i))
	{
		i->type = IDENTIFIER_TYPE_REGISTER;
		i->u.register_number = number;
	}
	else
	{
		LOG_ERROR("Identifier_Create() failed");
	}

	return i;
}


Identifier Identifier_NewMap(const char *map_name, const char *element_name)
{
	Identifier i = Identifier_Create();
	i->type = IDENTIFIER_TYPE_MAP;
	i->u.map = Map_Create(map_name, element_name);
	if(IS_NULL(i->u.map))
	{
		Identifier_Destroy(i);
		LOG_ERROR("Map_Create() failed");
		i = NULL;
	}
	return i;
}

Identifier Identifier_NewObject(Object object)
{
	Identifier i = Identifier_Create();
	if(!IS_NULL(i))
	{
		if(IS_NULL(object))
		{
			Identifier_Destroy(i);
			LOG_ERROR("Object passed is NULL");
			return NULL;
		}
		i->type = IDENTIFIER_TYPE_OBJECT;
		i->u.obj = object;
		i->u.obj->num_refs++;
	}
	else
	{
		LOG_ERROR("Identifier_Create() failed");
	}
	return i;
}



Identifier Identifier_Clone(Identifier a)
{
	Identifier i = NULL;
	switch(a->type)
	{
		case IDENTIFIER_TYPE_VARIABLE:
			{
				i = Identifier_NewVariable(a->u.variable_name);
				break;
			}
		case IDENTIFIER_TYPE_STRING:
			{
				i = Identifier_NewString(a->u.str);
				break;
			}
		case IDENTIFIER_TYPE_NUMBER:
			{
				i = Identifier_NewInteger(a->u.number);
				break;
			}
		case IDENTIFIER_TYPE_REGISTER:
			{
				i = Identifier_NewRegister(a->u.register_number);
				break;
			}
		case IDENTIFIER_TYPE_FLOAT:
			{
				i = Identifier_NewFloat(a->u.real);
				break;
			}
		case IDENTIFIER_TYPE_ARGUMENT:
			{
				i = Identifier_NewArgument(a->u.argument_number);
				break;
			}
		case IDENTIFIER_TYPE_OBJECT:
			{
				i = Identifier_NewObject(a->u.obj);
				break;
			}
		case IDENTIFIER_TYPE_MAP:
			{
				i = Identifier_Create();
				i->type = IDENTIFIER_TYPE_MAP;
				i->u.map = a->u.map;
				i->u.map->num_refs++;
				break;
			}
		default:
			{
				LOG_ERROR("unkown type %u", a->type);
				abort();
				break;
			}
	}
	return i;
}


void Identifier_Copy(Identifier dest, Identifier src)
{
	switch(src->type)
	{
		case IDENTIFIER_TYPE_STRING:
			{
				Identifier_SetString(dest, src->u.str);
				break;
			}
		case IDENTIFIER_TYPE_NUMBER:
			{
				Identifier_SetInt(dest, src->u.number);
				break;
			}
		case IDENTIFIER_TYPE_FLOAT:
			{
				Identifier_SetFloat(dest, src->u.real);
				break;
			}
		case IDENTIFIER_TYPE_OBJECT:
			{
				Identifier_SetObject(dest, src->u.obj);
				break;
			}
		default:
			{
				LOG_ERROR("unkown type %u", src->type);
				abort();
				break;
			}
	}
	return;
}

void Identifier_SetObject(Identifier dest, Object obj)
{	
	Identifier_Free(dest);
	dest->type = IDENTIFIER_TYPE_OBJECT;
	dest->u.obj = obj;
	dest->u.obj->num_refs++;
}
void Identifier_SetInt(Identifier a, int num)
{
	Identifier_Free(a);
	a->type = IDENTIFIER_TYPE_NUMBER;
	a->u.number = num;
}
void Identifier_SetString(Identifier a, char *str)
{
	Identifier_Free(a);
	a->type = IDENTIFIER_TYPE_STRING;
	a->u.str = (char*)Malloc(strlen(str)+1);
	strcpy(a->u.str, str);
}
void Identifier_SetFloat(Identifier a, double num)
{
	Identifier_Free(a);
	a->type = IDENTIFIER_TYPE_FLOAT;
	a->u.real = num;
}

Identifier Identifier_Create(void)
{
	Identifier id = NULL;
	id = (Identifier)Malloc(sizeof(struct _Identifier));
	if(IS_NULL(id))
	{
		LOG_ERROR("Malloc() failed");
	}
	else
	{
		memset(id, 0, sizeof(struct _Identifier));
		id->type = IDENTIFIER_TYPE_NUMBER;
	}
	return id;
}

void Identifier_Free(Identifier t)
{
	if(t->type == IDENTIFIER_TYPE_VARIABLE)
	{
		Free(t->u.str);
	}
	else if(t->type == IDENTIFIER_TYPE_STRING)
	{
		Free(t->u.variable_name);
	}
	else if(t->type == IDENTIFIER_TYPE_OBJECT)	
	{
		t->u.obj->num_refs--;
		if(t->u.obj->num_refs == 0)
		{
			t->u.obj->obj_delete(t->u.obj->priv_data);
			Free(t->u.obj);
		}
	}
	else if(t->type == IDENTIFIER_TYPE_MAP)
	{
		t->u.map->num_refs--;
		if(0 == t->u.map->num_refs)
		{
			Map_Delete(t->u.map);
		}
	}
	else if(t->type <= IDENTIFIER_TYPE_UNKNOWN_START || t->type >= IDENTIFIER_TYPE_UNKNOWN_END)
	{
		abort();
	}
	return;
}

void Identifier_Destroy(Identifier t)
{
	if(IS_NULL(t))
	{
		return;
	}
	Identifier_Free(t);
	memset(t, 0, sizeof(struct _Identifier));
	Free(t);
	return;
}


IdentifierStack IdentifierStack_Create(void)
{
	IdentifierStack is = (IdentifierStack)Malloc(sizeof(struct _IdentifierStack));
	if(IS_NULL(is))
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(struct _IdentifierStack));
		return NULL;
	}
	is->size = 25;
	is->top = -1;
	is->list = (Identifier*)Malloc(sizeof(Identifier) * is->size);
	if(IS_NULL(is->list))
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(Identifier) * is->size);
		Free(is);
		return NULL;
	}
	memset(is->list, 0, sizeof(Identifier) * is->size);
	return is;
}


STATUS IdentifierStack_Push(IdentifierStack is, Identifier A)
{
	if(IS_NULL(A))
	{
		LOG_ERROR("Trying to push NULL");
		return STATUS_FAILURE;
	}
	if(is->top == (is->size-1))
	{
		is->size += 25;
		is->list = (Identifier*)ReAlloc(is->list, sizeof(Identifier) * is->size);
		if(IS_NULL(is->list))
		{
			LOG_ERROR("ReAlloc(%u) failed", sizeof(Identifier) * is->size);
			return STATUS_FAILURE;
		}
		memset(&is->list[is->size - 25], 0, sizeof(Identifier) * (is->size-25));
	}
	++is->top;
	is->list[is->top] = Identifier_Clone(A);
	if(IS_NULL(is->list[is->top]))
	{
		return STATUS_FAILURE;
	}
	return STATUS_SUCCESS;
}

Identifier IdentifierStack_Pop(IdentifierStack is)
{
	Identifier d = NULL;
	if(is->top < 0)
	{
		LOG_ERROR("Stack_POp() failed as stack is empty");
		return NULL;
	}
	if(!IS_NULL(is->list[is->top]))
	{
		d = Identifier_Clone(is->list[is->top]);
		Identifier_Destroy(is->list[is->top]);
		is->list[is->top] = NULL;
		is->top--;
	}
	return d;
}

void IdentifierStack_Destroy(IdentifierStack is)
{
	int i = 0;
	for(i = 0; i <= is->top; i++)
	{
		if(!IS_NULL(is->list[i]))
			Identifier_Destroy(is->list[i]);
	}
	Free(is->list);
	Free(is);
}

Map Map_Create(const char *map_name, const char *element_name)
{
	Map map = (Map)Malloc(sizeof(struct _Map));
	if(!IS_NULL(map))
	{
		map->map_name = (char*)Malloc(strlen(map_name));
		map->element_name = (char*)Malloc(strlen(element_name));
		if(IS_NULL(map->map_name) || IS_NULL(map->element_name))
		{
			Free(map->map_name);		/*Free free's only if its not NULL*/
			Free(map->element_name);
			Free(map);
			LOG_ERROR("Malloc failed");
			return NULL;
		}
		strcpy(map->map_name, map_name);
		strcpy(map->element_name, element_name);
		map->num_refs = 1;
	}
	else
	{
		LOG_ERROR("Unable to create Map object");
	}
	return map;
}

void Map_Delete(Map m)
{
	Free(m->map_name);
	Free(m->element_name);
	Free(m);
}
