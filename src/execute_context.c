#include <string.h>
#include <stdarg.h>

#include "function.h"
#include "executable.h"


void RaiseException(Executable exe, const char *format, ...)
{
	va_list ap;
	char buffer[1024];
	LOG_ERROR("Exception Raised near line %d", exe->ec->cur_ptr->line_number);
	va_start(ap, format);
	vsnprintf(buffer, 1024, format, ap);
	LOG_ERROR("%s", buffer);
	exe->error_flag = 1;
}

void PrintBackTrace(Executable exe)
{
	int i = 0;
	LOG_ERROR("=========Printing Backtrace====\n\n");
	for(i = exe->ec_top; i >= 0; i--)
	{
		LOG_ERROR("%d> function %20s:%d", i,exe->ec_list[i]->func_name, exe->ec_list[i]->cur_ptr->line_number);
	}
	LOG_INFO_NL("\n\n");
	LOG_ERROR("=========End of Back Trace ====");
}

Identifier GetIdentifier(Executable exe, Identifier A)
{
	switch(A->type)
	{
		case IDENTIFIER_TYPE_STRING:
		case IDENTIFIER_TYPE_NUMBER:
		case IDENTIFIER_TYPE_OBJECT:
				return A;

		case IDENTIFIER_TYPE_REGISTER:
				return exe->ec->regs[A->u.register_number];

		case IDENTIFIER_TYPE_VARIABLE:
				{
					Identifier v = VariableList_FindVariable(exe->ec->local_variables, A->u.variable_name);
					if(IS_NULL(v))
					{
						RaiseException(exe, "VariableList_FindVariable('%s') failed hence stoping execution", A->u.variable_name);
						return NULL;
					}
					return v;
				}

		case IDENTIFIER_TYPE_ARGUMENT:
				{
					if(A->u.argument_number >= exe->ec->num_args)
					{
						RaiseException(exe, "Accessing argument %d which is not passed", A->u.argument_number);
						PrintBackTrace(exe);
						return NULL;
					}
					return exe->ec->args[A->u.argument_number];
				}
		case IDENTIFIER_TYPE_MAP_ELEMENT:
				{
					Identifier i;
					Identifier v = VariableList_FindVariable(exe->ec->local_variables, A->u.map_element->map_name);
					if(IS_NULL(v))
					{
						RaiseException(exe, "VariableList_FindVariable('%s') failed hence stoping execution", A->u.map_element->map_name);
						return NULL;
					}
					if(v->type != IDENTIFIER_TYPE_OBJECT || (strcmp(v->u.obj->type, "struct") != 0))
					{
						Identifier i;
						i = Map_Create();
						if(IS_NULL(i))
						{
							RaiseException(exe, "Map_Create() failed hence stoping execution", A->u.variable_name);
							return NULL;
						}
						Identifier_Copy(v, i);
						Identifier_Destroy(i);
					}
					i = Map_FindElement(v, A->u.map_element->element_name);
					if(IS_NULL(i))
					{
						RaiseException(exe, "Map_FindElement(%s, %s) failed hence stoping execution", A->u.variable_name);
						return NULL;
					}
					return i;
				}
		case IDENTIFIER_TYPE_ARRAY_ELEMENT:
				{
					Identifier i, tmp;
					Identifier v = VariableList_FindVariable(exe->ec->local_variables, A->u.array_element->array_name);
					if(IS_NULL(v))
					{
						RaiseException(exe, "VariableList_FindVariable('%s') failed hence stoping execution", A->u.array_element->array_name);
						return NULL;
					}
					if(v->type != IDENTIFIER_TYPE_OBJECT || (strcmp(v->u.obj->type, "array") != 0))
					{
						Identifier i;
						i = Array_Create();
						if(IS_NULL(i))
						{
							RaiseException(exe, "Array_Create() failed hence stoping execution", A->u.variable_name);
							return NULL;
						}
						Identifier_Copy(v, i);
						Identifier_Destroy(i);
					}
					tmp = A->u.array_element->idx;
					if(!(A->u.array_element->idx->type == IDENTIFIER_TYPE_STRING || A->u.array_element->idx->type == IDENTIFIER_TYPE_NUMBER))
					{
						tmp = GetIdentifier(exe,A->u.array_element->idx);
					}
					i = Array_FindIndex(v, tmp);
					if(IS_NULL(i))
					{
						if(STATUS_SUCCESS != Array_AddElement(v, tmp, NULL))
						{
							RaiseException(exe, "Array_FindElement failed hence stoping execution");
							return NULL;
						}
						i = Array_FindIndex(v, tmp);
					}					
					return i;
				}
		default:
				LOG_ERROR("type %d not supported", A->type); 
				abort();
	}
	return NULL;
}

void Execute_Mul(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;

	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;

	answer = A->u.number * B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Add(Executable exe, Identifier A, Identifier B, Identifier C)
{
	if(exe->error_flag)
	{
		LOG_ERROR("Addition could not be completed due to error");
		return;
	}

	if(A->type == IDENTIFIER_TYPE_NUMBER)
	{
		if(B->type == IDENTIFIER_TYPE_NUMBER)
		{
			int answer = 0;
			answer = A->u.number + B->u.number;
			Identifier_SetInt(C, answer);
		}
		else if(B->type == IDENTIFIER_TYPE_STRING)
		{
			int count = 0;
			char *str = NULL;
			count = snprintf(NULL, 0, "%d%s", A->u.number, B->u.str)+1;
			str = (char*)Malloc(count);
			if(!IS_NULL(str))
			{
				snprintf(str, count, "%d%s", A->u.number, B->u.str);
				Identifier_SetString(C, str);
				Free(str);
			}
			else
			{
				RaiseException(exe, "Malloc() failed");
				return;
			}

		}
		else
		{
			RaiseException(exe, "Unknown Id type %d for addition. Stopping Execution", B->type);
			return;
		}
	}
	else if(A->type == IDENTIFIER_TYPE_STRING)
	{
		int count = 0;
		char *str = NULL;
		if(B->type == IDENTIFIER_TYPE_NUMBER)
		{
			count = snprintf(NULL, 0, "%s%d", A->u.str, B->u.number)+1;
			str = (char*)Malloc(count);
			if(!IS_NULL(str))
			{
				snprintf(str, count, "%s%d", A->u.str, B->u.number);
				Identifier_SetString(C, str);
				Free(str);
			}
			else
			{
				RaiseException(exe, "Malloc() failed");
				return;
			}
		}
		else if(B->type == IDENTIFIER_TYPE_STRING)
		{
			count = snprintf(NULL, 0, "%s%s", A->u.str, B->u.str)+1;
			str = (char*)Malloc(count);
			if(!IS_NULL(str))
			{
				snprintf(str, count, "%s%s", A->u.str, B->u.str);
				Identifier_SetString(C, str);
				Free(str);
			}
			else
			{
				RaiseException(exe, "Malloc() failed");
				return;
			}
		}
		else
		{
			RaiseException(exe, "Unknown Id type %d for addition. Stopping Execution", B->type);
			return;
		}
	}
	else
	{
			RaiseException(exe, "Unknown Id type %d for addition. Stopping Execution", B->type);
			return;
	}
	return;
}
void Execute_Sub(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;

	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;

	answer = A->u.number - B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Gth(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;

	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;

	answer = A->u.number > B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Lth(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;

	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;

	answer = A->u.number < B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Gte(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;


	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;
	answer = A->u.number >= B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Lte(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;

	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;
	answer = A->u.number <= B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Cmp(Executable exe, Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	if(exe->error_flag)
	{
		LOG_ERROR("Cmp could not be completed due to error");
		return;
	}
	if(A->type == IDENTIFIER_TYPE_STRING && B->type == IDENTIFIER_TYPE_STRING)
	{
		answer = !strcmp(A->u.str, B->u.str);
	}
	else if(A->type == IDENTIFIER_TYPE_NUMBER && B->type == IDENTIFIER_TYPE_NUMBER)
	{
		answer = A->u.number == B->u.number;
	}
	else
	{
		RaiseException(exe, "Comparing type %d with %d. Stoping Execution", A->type, B->type);
		return;
	}
	Identifier_SetInt(C, answer);
}
void Execute_And(Executable exe, Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	if(exe->error_flag)
	{
		LOG_ERROR("And could not be completed due to error");
		return;
	}
	if(A->type == IDENTIFIER_TYPE_NUMBER && B->type == IDENTIFIER_TYPE_NUMBER)
	{
		answer = A->u.number && B->u.number;
	}
	else
	{
		RaiseException(exe, "Comparing type %d with %d. Stoping Execution", A->type, B->type);
		return;
	}

	Identifier_SetInt(C, answer);
}
void Execute_Or(Executable exe, Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	if(exe->error_flag)
	{
		LOG_ERROR("OR could not be completed due to error");
		return;
	}
	if(A->type == IDENTIFIER_TYPE_NUMBER && B->type == IDENTIFIER_TYPE_NUMBER)
	{
		answer = A->u.number || B->u.number;
	}
	else
	{
		RaiseException(exe, "Comparing type %d with %d. Stoping Execution", A->type, B->type);
		return;
	}

	Identifier_SetInt(C, answer);
}
void Execute_Neq(Executable exe, Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	if(exe->error_flag)
	{
		LOG_ERROR("NEQ could not be completed due to error");
		return;
	}
	if(A->type == IDENTIFIER_TYPE_STRING && B->type == IDENTIFIER_TYPE_STRING)
	{
		answer = strcmp(A->u.str, B->u.str);
	}
	else if(A->type == IDENTIFIER_TYPE_NUMBER && B->type == IDENTIFIER_TYPE_NUMBER)
	{
		answer = A->u.number != B->u.number;
	}
	else
	{
		RaiseException(exe, "Comparing type %d with %d. Stoping Execution", A->type, B->type);
		return;
	}

	Identifier_SetInt(C, answer);
}
void Execute_Equ(Executable exe, Identifier A, Identifier B, Identifier C)
{
	if(exe->error_flag)
	{
		LOG_ERROR("EQU could not be completed due to error");
		return;
	}
	if(B->type == IDENTIFIER_TYPE_STRING)
	{
		Identifier_SetString(A, B->u.str);
		Identifier_SetString(C, B->u.str);
	}
	else if(B->type == IDENTIFIER_TYPE_NUMBER)
	{
		Identifier_SetInt(A, B->u.number);
		Identifier_SetInt(C, B->u.number);
	}
	else if(B->type == IDENTIFIER_TYPE_OBJECT)
	{
        /*if A==B then a crash will happen because that corrsponds to
         * a==a if a is object then before assigning a to a we first free a 
         * and the copy a to a. So copy operation becomes invalid and might lead
         * to crash*/
        if(A != B)
        {            
            Identifier_SetObject(A, B->u.obj);
        }
		Identifier_SetObject(C, B->u.obj);
	}
	else
	{
		RaiseException(exe, "Unknown Id type %d for assignment. Stopping Execution", B->type);
		return;
	}
}

void Execute_Not(Executable exe, Identifier B, Identifier C)
{
	if(exe->error_flag)
	{
		LOG_ERROR("NOT could not be completed due to error");
		return;
	}
	if(B->type == IDENTIFIER_TYPE_NUMBER)
	{
		Identifier_SetInt(C, !B->u.number);
	}
	else
	{
		RaiseException(exe, "Unknown Id type %d for NOT operation. Stopping Execution", B->type);
		return;
	}
}
void Execute_Jz(Executable exe, Identifier A, int label_number)
{
	if(IS_NULL(A)) return;
	if(A->u.number == 0)
	{
	//	LOG_INFO("label_%d address is %#x", label_number, exe->label_list[label_number]);
		exe->ec->cur_ptr = (ByteCode)exe->label_list[label_number];
	}
}

void Execute_Jump(Executable exe, int label_number)
{
	exe->ec->cur_ptr = (ByteCode)exe->label_list[label_number];
}

void Execute_Call(Executable exe, const char *fun_name, int num_args, Identifier result)
{
	int i;
	Identifier *args;
	FunctionList func;
	func = FunctionList_FindFunction(exe->func_list, fun_name);
	if(IS_NULL(func))
	{
		RaiseException(exe, "func '%s' not found", fun_name);
		return;
	}

	if(exe->ec_top == (exe->ec_size-1))
	{
		if(STATUS_FAILURE == Executable_GrowExecutionContext(exe))
		{
			LOG_ERROR("Executable_GrowExecutionContext() failed");
			return ;
		}
	}

	args = (Identifier*)Malloc(sizeof(Identifier) * (num_args+1));
	args[0] = Identifier_NewInteger(num_args);
	for(i = num_args; i != 0; i--)
	{
		Identifier p = IdentifierStack_Pop(exe->is);
		if(IS_NULL(p))
		{
			RaiseException(exe, "POP from stack failed. This should not happen");
			return;
		}
		args[i] = GetIdentifier(exe, p);
	}

	if(func->type == FUNCTION_TYPE_NATIVE)
	{
		Identifier r;
		r = (*func->u.nFunc)(args, num_args);
		for(i = 0; i < num_args+1; i++)
		{
			Identifier_Destroy(args[i]);
		}
		Free(args);
		if(!IS_NULL(r))
		{
			Identifier_Copy(result, r);
			Identifier_Destroy(r);
		}
		else
		{
			Identifier_SetInt(result, 0);
		}
	}
	else
	{
		exe->ec = exe->ec_list[++exe->ec_top] = ExecutionContext_Create(((ByteCode)(func->u.address)));
		exe->ec->num_args = num_args+1;
		exe->ec->args = args;
		exe->ec->func_name =func->func_name;
	}
}


STATUS Execute_Return(Executable exe, Identifier ret)
{
	Identifier ret_value = Identifier_Clone(ret);
	ExecutionContext_Destroy(exe->ec);
	exe->ec_top--;
	if(-1 == exe->ec_top)
	{
		Identifier_Copy(exe->ret_value, ret_value);
		exe->ec = NULL;
//		LOG_INFO("End of program");
	}
	else
	{
		exe->ec = exe->ec_list[exe->ec_top];
		Identifier_Copy(GetIdentifier(exe, exe->ec->cur_ptr->C), ret_value);
	}
	Identifier_Destroy(ret_value);
	return STATUS_SUCCESS;
}

STATUS ExecutionContext_Execute(Executable exe, const char *func_name, Identifier i)
{
	FunctionList func;

	exe->ec = exe->ec_list[++exe->ec_top] = ExecutionContext_Create(NULL);
	func = FunctionList_FindFunction(exe->func_list, func_name);
	if(IS_NULL(func) || (func->type != FUNCTION_TYPE_LOCAL))
	{
		LOG_ERROR("func '%s' not found", func_name);
		return STATUS_FAILURE;
	}
	exe->ec->num_args = IS_NULL(i) ? 1 : 2;
	exe->ec->args = (Identifier*)Malloc(sizeof(Identifier) * exe->ec->num_args);
	exe->ec->args[0] = Identifier_NewInteger(exe->ec->num_args-1);
	if(!IS_NULL(i))
	{
		exe->ec->args[1] = Identifier_Clone(i);
	}
	exe->ec->func_name =func->func_name;
	exe->ec->cur_ptr = ((ByteCode)(func->u.address))->next;
	while(!IS_NULL(exe->ec->cur_ptr) && (exe->error_flag == 0))
	{
//		LOG_INFO("Executing on line %d", exe->ec->cur_ptr->line_number);
		switch(exe->ec->cur_ptr->cmd)
		{
			case MUL:
					Execute_Mul(GetIdentifier(exe, exe->ec->cur_ptr->A),
								GetIdentifier(exe, exe->ec->cur_ptr->B),
								GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case ADD:
					Execute_Add(exe, GetIdentifier(exe, exe->ec->cur_ptr->A),
									 GetIdentifier(exe, exe->ec->cur_ptr->B),
									 GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case SUB:
					Execute_Sub(GetIdentifier(exe, exe->ec->cur_ptr->A),
								GetIdentifier(exe, exe->ec->cur_ptr->B),
								GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case GTH:
					Execute_Gth(GetIdentifier(exe, exe->ec->cur_ptr->A),
								GetIdentifier(exe, exe->ec->cur_ptr->B),
								GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case LTH:
					Execute_Lth(GetIdentifier(exe, exe->ec->cur_ptr->A),
								GetIdentifier(exe, exe->ec->cur_ptr->B),
								GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case GTE:
					Execute_Gte(GetIdentifier(exe, exe->ec->cur_ptr->A),
								GetIdentifier(exe, exe->ec->cur_ptr->B),
								GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case LTE:
					Execute_Lte(GetIdentifier(exe, exe->ec->cur_ptr->A),
								GetIdentifier(exe, exe->ec->cur_ptr->B),
								GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case OR:
				Execute_Or(exe, GetIdentifier(exe, exe->ec->cur_ptr->A),
								 GetIdentifier(exe, exe->ec->cur_ptr->B),
								 GetIdentifier(exe, exe->ec->cur_ptr->C));
				break;
			case AND:
				Execute_And(exe, GetIdentifier(exe, exe->ec->cur_ptr->A),
								 GetIdentifier(exe, exe->ec->cur_ptr->B),
								 GetIdentifier(exe, exe->ec->cur_ptr->C));
				break;
			case CMP:
					Execute_Cmp(exe, GetIdentifier(exe, exe->ec->cur_ptr->A),
									 GetIdentifier(exe, exe->ec->cur_ptr->B),
									 GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case NEQ:
					Execute_Neq(exe, GetIdentifier(exe, exe->ec->cur_ptr->A),
									 GetIdentifier(exe, exe->ec->cur_ptr->B),
									 GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case EQU:
					Execute_Equ(exe, GetIdentifier(exe, exe->ec->cur_ptr->A),
									 GetIdentifier(exe, exe->ec->cur_ptr->B),
									 GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case NOT:
					Execute_Not(exe, GetIdentifier(exe, exe->ec->cur_ptr->B),
									 GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case JZ:
					Execute_Jz(exe, GetIdentifier(exe, exe->ec->cur_ptr->A), exe->ec->cur_ptr->u.label_number);
					break;
			case JUMP:
					Execute_Jump(exe, exe->ec->cur_ptr->u.label_number);
					break;
			case PUSH:
					{
						Identifier id = GetIdentifier(exe, exe->ec->cur_ptr->A);
						if(!IS_NULL(id))
						{
							IdentifierStack_Push(exe->is, id);
						}
						break;
					}
			case CALL:
					{
						Execute_Call(exe,  exe->ec->cur_ptr->A->u.str, exe->ec->cur_ptr->u.num_arguments, GetIdentifier(exe, exe->ec->cur_ptr->C));
						break;
					}
			case VAR:
					{
						if(STATUS_SUCCESS != VariableList_AddVariable(exe->ec->local_variables,
																	  exe->ec->cur_ptr->A->u.variable_name,
																	  GetIdentifier(exe, exe->ec->cur_ptr->B)))
						{
							RaiseException(exe, "VariableList_AddVariable(%s) failed", exe->ec->cur_ptr->A->u.variable_name);
							goto SCRIPT_ERROR;
						}
						break;
					}
			case RETURN:
					{
						Execute_Return(exe, GetIdentifier(exe, exe->ec->cur_ptr->A));
						break;
					}
			default:
					LOG_ERROR("CMD %u not found", exe->ec->cur_ptr->cmd);
					abort();
					break;
		}
		if(IS_NULL(exe->ec) || IS_NULL(exe->ec->cur_ptr))
		{
			break;
		}
		exe->ec->cur_ptr = exe->ec->cur_ptr->next;
	}
SCRIPT_ERROR:
	return STATUS_SUCCESS;
}

void ExecutionContext_Destroy(ExecutionContext ec)
{
	int i;
	for(i = 0; i < ec->num_regs; i++)
	{
		Identifier_Destroy(ec->regs[i]);
	}
	Free(ec->regs);
	VariableList_Destroy(ec->local_variables);
	if(!IS_NULL(ec->args))
	{
		for(i = 0; i < ec->num_args; i++)
		{
			Identifier_Destroy(ec->args[i]);
		}
		Free(ec->args);
	}
	Free(ec);
}
ExecutionContext ExecutionContext_Create(ByteCode cur_ptr)
{
	ExecutionContext ec = NULL;

	int i = 0;
	ec = (ExecutionContext)Malloc(sizeof(struct _ExecutionContext));
	if(IS_NULL(ec))
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(struct _ExecutionContext));
		return NULL;
	}
	memset(ec, 0, sizeof(struct _ExecutionContext));
	ec->cur_ptr = cur_ptr;

	ec->num_regs = 25;
	ec->regs = (Identifier*)Malloc(sizeof(struct _Identifier) * ec->num_regs);
	if(IS_NULL(ec->regs))
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(struct _Identifier) * ec->num_regs);
		Free(ec);
		return NULL;
	}
	memset(ec->regs, 0, sizeof(struct _Identifier) * ec->num_regs);
	for(i =0 ; i < ec->num_regs; i++)
	{
		ec->regs[i] = Identifier_NewInteger(0);
	}

	ec->local_variables = VariableList_Create();
	if(IS_NULL(ec->local_variables))
	{
		Free(ec->regs);
		Free(ec);
		LOG_ERROR("Variable_Create() failed");
		return NULL;
	}

	return ec;
}

int ExecutionContext_GetReturnValue(Executable exe)
{
	if(IS_NULL(exe->ret_value))
	{
		return 0;
	}
	switch(exe->ret_value->type)
	{
		case IDENTIFIER_TYPE_NUMBER:
				return exe->ret_value->u.number;
		case IDENTIFIER_TYPE_STRING:
				return atoi(exe->ret_value->u.str);
		default:
				return 0;
	}
	return 0;
}
