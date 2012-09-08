#include <string.h>
#include "function.h"
#include "executable.h"

Identifier GetIdentifier(Executable exe, Identifier A)
{
	switch(A->type)
	{
		case IDENTIFIER_TYPE_FLOAT:
		case IDENTIFIER_TYPE_STRING:
		case IDENTIFIER_TYPE_NUMBER:
				return A;

		case IDENTIFIER_TYPE_REGISTER:
				return exe->ec->regs[A->u.argument_number]; 

		case IDENTIFIER_TYPE_VARIABLE:
				{
					Identifier v = VariableList_FindVariable(exe->ec->local_variables, A->u.variable_name);
					if(IS_NULL(v))
					{
						LOG_ERROR("VariableList_FindVariable('%s') failed hence stoping execution", A->u.variable_name);
						exe->error_flag = 1;
					}
					return v;
				}

		case IDENTIFIER_TYPE_ARGUMENT:
		case IDENTIFIER_TYPE_UNKNOWN:
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
void Execute_Add(Identifier A, Identifier B, Identifier C)  
{
	int answer = 0;

	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;

	answer = A->u.number + B->u.number;
	Identifier_SetInt(C, answer);
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
void Execute_Cmp(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	
	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;
	answer = A->u.number == B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Neq(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	
	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;
	answer = A->u.number == B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Equ(Identifier A, Identifier B, Identifier C)
{
	
	if(IS_NULL(A) || IS_NULL(B) || IS_NULL(C)) return;
	Identifier_SetInt(A, B->u.number);
	Identifier_SetInt(C, B->u.number);
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
	FunctionList func;
	func = FunctionList_FindFunction(exe->func_list, fun_name);
	if(IS_NULL(func))
	{
		LOG_ERROR("func '%s' not found", fun_name);
		abort();
	}
	
	if(exe->ec_top == (exe->ec_size-1))
	{
		if(STATUS_FAILURE == Executable_GrowExecutionContext(exe))
		{
			LOG_ERROR("Executable_GrowExecutionContext() failed");
			return ;
		}
	}
	exe->ec = exe->ec_list[++exe->ec_top] = ExecutionContext_Create(NULL);
	exe->ec->num_args = num_args+1; 
	exe->ec->args = (Identifier*)Malloc(sizeof(Identifier) * exe->ec->num_args);

	exe->ec->args[0] = Identifier_NewInteger(num_args);
	for(i = num_args; i != 0; i--)
	{
		exe->ec->args[i] = GetIdentifier(exe, IdentifierStack_Pop(exe->is));
	}

	if(func->type == FUNCTION_TYPE_NATIVE)
	{
		Identifier r;
		r = (*func->u.nFunc)(exe->ec->args, num_args);
		ExecutionContext_Destroy(exe->ec);
		exe->ec_top--;
		exe->ec = exe->ec_list[exe->ec_top];
		if(!IS_NULL(r))
		{
			Identifier_Copy(r,	result);
			Identifier_Destroy(r);
		}
		else
		{
			Identifier_SetInt(result, 0);
		}
	}
}
STATUS ExecutionContext_Execute(Executable exe)
{
	if(exe->ec_top == (exe->ec_size-1))
	{
		if(STATUS_FAILURE == Executable_GrowExecutionContext(exe))
		{
			LOG_ERROR("Executable_GrowExecutionContext() failed");
			return STATUS_FAILURE;
		}
	}
	exe->ec = exe->ec_list[++exe->ec_top] = ExecutionContext_Create(exe->first->next);
	
	while(!IS_NULL(exe->ec->cur_ptr) && (exe->error_flag == 0))
	{
		switch(exe->ec->cur_ptr->cmd)
		{
			case MUL:
					Execute_Mul(GetIdentifier(exe, exe->ec->cur_ptr->A), 
								GetIdentifier(exe, exe->ec->cur_ptr->B), 
								GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case ADD:
					Execute_Add(GetIdentifier(exe, exe->ec->cur_ptr->A), 
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
			case CMP:
					Execute_Cmp(GetIdentifier(exe, exe->ec->cur_ptr->A), 
								GetIdentifier(exe, exe->ec->cur_ptr->B), 
								GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case NEQ:
					Execute_Neq(GetIdentifier(exe, exe->ec->cur_ptr->A), 
								GetIdentifier(exe, exe->ec->cur_ptr->B), 
								GetIdentifier(exe, exe->ec->cur_ptr->C));					
					break;					
			case EQU:
					Execute_Equ(GetIdentifier(exe, exe->ec->cur_ptr->A), 
								GetIdentifier(exe, exe->ec->cur_ptr->B), 
								GetIdentifier(exe, exe->ec->cur_ptr->C));
					break;
			case JZ:
					Execute_Jz(exe, GetIdentifier(exe, exe->ec->cur_ptr->A), exe->ec->cur_ptr->u.label_number);
					break;
			case JUMP:
					Execute_Jump(exe, exe->ec->cur_ptr->u.label_number);
					break;
			case PUSH:
					IdentifierStack_Push(exe->is, GetIdentifier(exe, exe->ec->cur_ptr->A));
					break;
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
							LOG_ERROR("VariableList_AddVariable(%s) failed", exe->ec->cur_ptr->A->u.variable_name);
							exe->error_flag = 1;
							goto SCRIPT_ERROR;
						}
						break;
					}
			default:
					LOG_ERROR("CMD %u not found", exe->ec->cur_ptr->cmd);
					abort();
					break;
		}
		if(IS_NULL(exe->ec->cur_ptr))
		{
			break;
		}
		exe->ec->cur_ptr = exe->ec->cur_ptr->next;
	}
SCRIPT_ERROR:
	ExecutionContext_Destroy(exe->ec);
	exe->ec_top--;
	exe->ec = NULL;
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

	ec->local_variables = VariableList_Create("\0");
	if(IS_NULL(ec->local_variables))
	{
		Free(ec->regs);
		Free(ec);
		LOG_ERROR("Variable_Create() failed");
		return NULL;
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
		LOG_ERROR("Variable('%s') not found", variable_name);
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
	LOG_ERROR("Variable('%s') not found", variable_name);
	return NULL;
}

STATUS VariableList_AddVariable(VariableList vl, const char *variable_name, Identifier v)
{
	int flag;
	VariableList new_var = vl->next, tmp1_vl;

	if(strlen(variable_name) < 1)
	{
		LOG_ERROR("variable_name is of size 0");
		return STATUS_FAILURE;
	}

	if(IS_NULL(new_var))
	{
		new_var = VariableList_Create(variable_name);
		if(!IS_NULL(new_var))
		{
			new_var->id = Identifier_Clone(v);
			new_var->next = NULL;
			new_var->variable_name = (char*)Malloc(strlen(variable_name)+1);
			memcpy(new_var->variable_name, variable_name, strlen(variable_name));
			vl->next = new_var;
			return STATUS_SUCCESS;
		}
		else
		{
			LOG_ERROR("VariableList_Create() Failed");
			return STATUS_FAILURE;
		}
	}

	flag = 1;
	do
	{
		int tmp = strcasecmp(new_var->variable_name, variable_name);
		if(tmp > 0)
		{
			flag = 2;
			break;
		}
		else if(tmp == 0)
		{
			LOG_ERROR("variable with name '%s' already declared", variable_name);
			return STATUS_FAILURE;
		}
		tmp1_vl = new_var;
		new_var = new_var->next;
	}
	while(!IS_NULL(new_var));

	new_var = VariableList_Create(variable_name);
	if(!IS_NULL(new_var))
	{
		new_var->id = Identifier_Clone(v);
		new_var->next = NULL;
		new_var->variable_name = (char*)Malloc(strlen(variable_name)+1);
		memcpy(new_var->variable_name, variable_name, strlen(variable_name));
		if(flag == 1)
		{
			tmp1_vl->next = new_var;
		}
		else if(flag == 2)
		{
			new_var->next = tmp1_vl->next;
			tmp1_vl->next = new_var;
		}
		return STATUS_SUCCESS;
	}
	LOG_ERROR("VariableList_Create() Failed");
	return STATUS_FAILURE;
}
