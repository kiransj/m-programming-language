#include <string.h>
#include "tokenizer.h"
#include "util.h"
#include "compiler.h"

void Identifier_to_str(Identifier id, char * const buffer, const int size)
{
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
				abort();
				break;
			}
	}
	return i;

}

Identifier Identifier_Create(void)
{
	Identifier id = NULL;
	id = (Identifier)Malloc(sizeof(struct _Identifier));
	if(IS_NULL(id))
	{
		LOG_ERROR("Malloc() failed");
	}
	return id;
}

void Identifier_Destroy(Identifier t)
{
#if 0
	char buf[100];
	Identifier_to_str(t, buf, 100);
	LOG_ERROR("deleting %s", buf);
#endif
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
