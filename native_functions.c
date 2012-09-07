#include "util.h"
#include "executable.h"
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

void Register_Native_Functions(Executable exe)
{
	Executable_AddNativeFunction(exe, "output", Function_Output);
	Executable_AddNativeFunction(exe, "max", Function_Max);
}
