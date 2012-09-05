#include "tokenzier.h"
#include "util.h"

Identifier Identifier_NewString(const char *str)
{
	Identifier i;	
	i.u.str = Malloc(strlen(str)+1);
	if(!IS_NULL())
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

Identifier Identifier_NewVariable(const char *variable_name);
{
	Identifier i;	
	i.u.variable_name= Malloc(strlen(str)+1);
	if(!IS_NULL())
	{		
		i.type = IDENTIFIER_TYPE_Variable;
		strcpy(i.u.variable_name, str);
	}
	else
	{
		LOG_ERROR("Malloc(%d) failed", strlen(str)+1);
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
Identifier Identifier_NewInteger(const int number);
{
	Identifier i;	
	i.type = IDENTIFIER_TYPE_NUMBER;
	i.u.number = number;
	return i;
}

void Varable_Destroy(Identifier t)
{
	if(t.type == IDENTIFIER_TYPE_Variable ||)
	{
		Free(t.u.str);
	}
	else if(t.type == IDENTIFIER_TYPE_STRING)
	{
		Free(t.u.variable_name);
	}
	memset(t, 0, sizeof(struct _Identifier));
	return;
}
