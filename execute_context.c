#include <string.h>
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
				return VariableList_FindVariable(exe->ec->local_variables, A->u.variable_name);

		case IDENTIFIER_TYPE_ARGUMENT:
		case IDENTIFIER_TYPE_UNKNOWN:
				abort();
	}
	return NULL;
}

void Execute_Mul(Identifier A, Identifier B, Identifier C)  
{
	int answer = 0;
	answer = A->u.number * B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Add(Identifier A, Identifier B, Identifier C)  
{
	int answer = 0;
	answer = A->u.number + B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Sub(Identifier A, Identifier B, Identifier C)  
{
	int answer = 0;
	answer = A->u.number - B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Gth(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	answer = A->u.number > B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Lth(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	answer = A->u.number < B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Gte(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	answer = A->u.number >= B->u.number;	
	Identifier_SetInt(C, answer);
}
void Execute_Lte(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	answer = A->u.number <= B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Cmp(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	answer = A->u.number == B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Neq(Identifier A, Identifier B, Identifier C)
{
	int answer = 0;
	answer = A->u.number == B->u.number;
	Identifier_SetInt(C, answer);
}
void Execute_Equ(Identifier A, Identifier B, Identifier C)
{
	Identifier_SetInt(A, B->u.number);
	Identifier_SetInt(C, B->u.number);
	LOG_INFO_NL("Value = %u", B->u.number);
}

void Execute_Jz(Executable exe, Identifier A, int label_number)
{
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

STATUS ExecutionContext_Execute(Executable exe)
{
	ExecutionContext ec;
	ec = exe->ec = ExecutionContext_Create(exe->first->next);
	
	while(!IS_NULL(ec->cur_ptr))
	{
		switch(ec->cur_ptr->cmd)
		{
			case MUL:
					Execute_Mul(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));
					break;
			case ADD:
					Execute_Add(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));
					break;
			case SUB:
					Execute_Sub(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));
					break;				
			case GTH:
					Execute_Gth(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));
					break;
			case LTH:
					Execute_Lth(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));				
					break;			
			case GTE:
					Execute_Gte(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));
					break;
			case LTE:
					Execute_Lte(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));
					break;
			case CMP:
					Execute_Cmp(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));
					break;
			case NEQ:
					Execute_Neq(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));					
					break;					
			case EQU:
					Execute_Equ(GetIdentifier(exe, ec->cur_ptr->A), 
								GetIdentifier(exe, ec->cur_ptr->B), 
								GetIdentifier(exe, ec->cur_ptr->C));
					break;
			case JZ:
					Execute_Jz(exe, GetIdentifier(exe, ec->cur_ptr->A), ec->cur_ptr->u.label_number);
					break;
			case JUMP:
					Execute_Jump(exe, ec->cur_ptr->u.label_number);
					break;				
			default:
					LOG_ERROR("CMD %u not found", ec->cur_ptr->cmd);
					abort();
					break;
		}
		ec->cur_ptr = ec->cur_ptr->next;
	}
	ExecutionContext_Destroy(ec);
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
	Free(ec);
}
ExecutionContext ExecutionContext_Create(ByteCode cur_ptr)
{
	ExecutionContext ec = NULL;
	if(!IS_NULL(cur_ptr))
	{
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
	}
	else
	{
		LOG_ERROR("Trying to create a ExecutionContext pointing to NULL");
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
		tmp_vl = VariableList_Create(variable_name);
		if(!IS_NULL(tmp_vl))
		{
			tmp_vl->id = Identifier_NewInteger(0);
			tmp_vl->next = NULL;
			tmp_vl->variable_name = (char*)Malloc(strlen(variable_name)+1);
			memcpy(tmp_vl->variable_name, variable_name, strlen(variable_name));
			vl->next = tmp_vl;
		}
		else
		{
			LOG_ERROR("VariableList_Create() Failed");
			return NULL;
		}
	}

	flag = 1;
	do
	{
		int tmp = strcmp(tmp_vl->variable_name, variable_name);
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
	else
	{
		tmp_vl = VariableList_Create(variable_name);
		if(!IS_NULL(tmp_vl))
		{
			tmp_vl->id = Identifier_NewInteger(0);
			tmp_vl->next = NULL;
			tmp_vl->variable_name = (char*)Malloc(strlen(variable_name)+1);
			memcpy(tmp_vl->variable_name, variable_name, strlen(variable_name));
			if(flag == 1)
			{
				tmp1_vl->next = tmp_vl;
			}
			else if(flag == 2)
			{
				tmp_vl->next = tmp1_vl->next;
				tmp1_vl->next = tmp_vl;
			}
			return tmp_vl->id;
		}
		else
		{
			LOG_ERROR("VariableList_Create() Failed");
			return NULL;
		}
	}
	return NULL;
}
