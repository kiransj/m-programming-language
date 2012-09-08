#include <string.h>
#include "tokenizer.h"
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

		case IDENTIFIER_TYPE_UNKNOWN: 		
		default: 
			strcpy(buffer, "unknown"); 
			return;
	}
	return;
}
char* IdentifierType_str(IdentifierType type)
{
	switch(type)
	{
		case IDENTIFIER_TYPE_UNKNOWN:	return "unknown";
		case IDENTIFIER_TYPE_NUMBER:	return "int";
		case IDENTIFIER_TYPE_FLOAT:		return "float";
		case IDENTIFIER_TYPE_STRING:	return "string";
		case IDENTIFIER_TYPE_ARGUMENT:	return "arg";
		case IDENTIFIER_TYPE_VARIABLE:	return "var";										
		case IDENTIFIER_TYPE_REGISTER:	return "reg";
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
			LOG_ERROR("Malloc(%d) failed", strlen(str)+1);
			i->type = IDENTIFIER_TYPE_UNKNOWN;
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
		default:
			{
				LOG_ERROR("unkown type %u", a->type);
				abort();
				break;
			}
	}
	return i;
}

void Identifier_Copy(Identifier src, Identifier dest)
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
		default:
			{
				LOG_ERROR("unkown type %u", src->type);
				abort();
				break;
			}
	}
	return;
}
void Identifier_SetInt(Identifier a, int num)
{
	if(a->type == IDENTIFIER_TYPE_STRING)
		Free(a->u.str);
	if(a->type == IDENTIFIER_TYPE_VARIABLE)
		Free(a->u.variable_name);

	a->type = IDENTIFIER_TYPE_NUMBER;
	a->u.number = num;
}
void Identifier_SetString(Identifier a, char *str)
{
	if(a->type == IDENTIFIER_TYPE_STRING)
		Free(a->u.str);
	if(a->type == IDENTIFIER_TYPE_VARIABLE)
		Free(a->u.variable_name);

	a->type = IDENTIFIER_TYPE_STRING;
	a->u.str = (char*)Malloc(strlen(str)+1);
	strcpy(a->u.str, str);
}
void Identifier_SetFloat(Identifier a, double num)
{
	if(a->type == IDENTIFIER_TYPE_STRING)
		Free(a->u.str);
	if(a->type == IDENTIFIER_TYPE_VARIABLE)
		Free(a->u.variable_name);
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
	}
	return id;
}

void Identifier_Destroy(Identifier t)
{
	if(IS_NULL(t))
	{
		return;
	}
	if(t->type == IDENTIFIER_TYPE_VARIABLE)
	{
		Free(t->u.str);
	}
	else if(t->type == IDENTIFIER_TYPE_STRING)
	{
		Free(t->u.variable_name);
	}
	else if(t->type == IDENTIFIER_TYPE_UNKNOWN)
	{
		abort();
	}
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
