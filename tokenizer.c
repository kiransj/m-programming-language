#include <string.h>
#include "tokenizer.h"
#include "util.h"

Identifier Identifier_NewString(const char *str)
{
	Identifier i;	
	i.u.str = Malloc(strlen(str)+1);
	if(!IS_NULL(i.u.str))
	{		
		i.type = IDENTIFIER_TYPE_STRING;
		strcpy(i.u.str, str);
	}
	else
	{
		LOG_ERROR("Malloc(%d) failed", strlen(str)+1);
		i.type = IDENTIFIER_TYPE_STRING;
	}
	return i;
}

Identifier Identifier_NewVariable(const char *variable_name)
{
	Identifier i;	
	i.u.variable_name= Malloc(strlen(variable_name)+1);
	if(!IS_NULL(i.u.variable_name))
	{		
		i.type = IDENTIFIER_TYPE_VARIABLE;
		strcpy(i.u.variable_name, variable_name);
	}
	else
	{
		LOG_ERROR("Malloc(%d) failed", strlen(variable_name)+1);
		i.type = IDENTIFIER_TYPE_STRING;
	}
	return i;
}

Identifier Identifier_NewFloat(const double real)
{
	Identifier i;	
	i.type = IDENTIFIER_TYPE_FLOAT;
	i.u.real = real;
	return i;
}

Identifier Identifier_NewArgument(const int argument_number)
{
	Identifier i;	
	i.type = IDENTIFIER_TYPE_ARGUMENT;
	i.u.argument_number = argument_number;
	return i;
}
Identifier Identifier_NewInteger(const int number)
{
	Identifier i;	
	i.type = IDENTIFIER_TYPE_NUMBER;
	i.u.number = number;
	return i;
}

void Indentifier_Destroy(Identifier t)
{
	if(t.type == IDENTIFIER_TYPE_VARIABLE)
	{
		Free(t.u.str);
	}
	else if(t.type == IDENTIFIER_TYPE_STRING)
	{
		Free(t.u.variable_name);
	}
	memset(&t, 0, sizeof(struct _Identifier));
	return;
}
