#include <string.h>
#include "executable.h"


static ByteCode ByteCode_Create(void)
{
	ByteCode bc;
	bc = (ByteCode)Malloc(sizeof(struct _ByteCode));
	if(!IS_NULL(bc))
	{
		memset(bc, 0, sizeof(struct _ByteCode));
	}
	else
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(struct _ByteCode));
	}
	return bc;
}
static void ByteCode_Destroy(ByteCode bc)
{
	if(!IS_NULL(bc->A)) Identifier_Destroy(bc->A);
	if(!IS_NULL(bc->B)) Identifier_Destroy(bc->B);
	if(!IS_NULL(bc->C)) Identifier_Destroy(bc->C);
	Free(bc);
}

Executable Executable_Create(void)
{
	Executable exe;
	exe = (Executable)Malloc(sizeof(struct _Executable));
	if(!IS_NULL(exe))
	{
		memset(exe, 0, sizeof(struct _Executable));
		exe->label_size = 25;
		exe->label_list = (unsigned int *)Malloc(sizeof(unsigned int) * exe->label_size);
		if(IS_NULL(exe->label_list))
		{
			LOG_ERROR("Malloc(%u) failed", sizeof(unsigned int) * exe->label_size);
			Free(exe);
			return NULL;
		}
		memset(exe->label_list, 0, sizeof(unsigned int) * exe->label_size);

		exe->first = exe->last = ByteCode_Create();
		if(IS_NULL(exe->first))
		{
			LOG_ERROR("ByteCode_Create() failed");
			Free(exe->label_list);
			Free(exe);
			return NULL;
		}

		exe->is = IdentifierStack_Create();
		if(IS_NULL(exe->is))
		{
			LOG_ERROR("IdentifierStack_Create() failed");
			Free(exe->first);
			Free(exe->label_list);
			Free(exe);
			return NULL;
		}

		exe->ec_top = -1;
		exe->ec_size = 25;
		exe->ec_list = (ExecutionContext*)Malloc(sizeof(ExecutionContext) * exe->ec_size);
		if(IS_NULL(exe->ec_list))
		{
			LOG_ERROR("Malloc failed to create ExecutionContextList");
			Free(exe->first);
			Free(exe->label_list);
			IdentifierStack_Destroy(exe->is);
			Free(exe);
			return NULL;
		}

		exe->func_list = FunctionList_Create();
		if(IS_NULL(exe->func_list))
		{
			LOG_ERROR("Malloc failed to create ExecutionContextList");
			Free(exe->first);
			Free(exe->label_list);
			IdentifierStack_Destroy(exe->is);
			Free(exe->ec_list);
			Free(exe);
			return NULL;
		}
		exe->ret_value = Identifier_Create();
	}
	else
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(struct _Executable));
	}
	return exe;
}

STATUS Executable_AddNativeFunction(Executable exe, const char *func_name,  NativeFunction nf)
{
	return	FunctionList_AddNativeFunction(exe->func_list, func_name, nf);
}

STATUS Executable_GrowExecutionContext(Executable exe)
{
	exe->ec_size += 25;
	exe->ec_list = (ExecutionContext*)ReAlloc(exe->ec_list, sizeof(ExecutionContext) * exe->ec_size);
	if(IS_NULL(exe->ec_list))
	{
		LOG_ERROR("ReAlloc failed. Unable to grow ExecutionContextList");
		return STATUS_FAILURE;
	}
	memset(&exe->ec_list[exe->ec_size-25], 0, sizeof(ExecutionContext) * (exe->ec_size - 25));
	return STATUS_SUCCESS;
}

void Executable_Destroy(Executable exe)
{
	ByteCode tmp = exe->first, tmp1;
	while(!IS_NULL(tmp))
	{
		tmp1 = tmp->next;
		ByteCode_Destroy(tmp);
		tmp = tmp1;
	}
	if(!IS_NULL(exe->label_list)) Free(exe->label_list);
	if(!IS_NULL(exe->is)) IdentifierStack_Destroy(exe->is);
	if(!IS_NULL(exe->ec_list)) Free(exe->ec_list);
	if(!IS_NULL(exe->func_list)) FunctionList_Destroy(exe->func_list);
	if(!IS_NULL(exe->ret_value)) Identifier_Destroy(exe->ret_value);
	Free(exe);
}

STATUS Executable_AddCmd(Executable exe, CompilerCmd cmd, Identifier a, Identifier b, Identifier c, int number)
{
	Identifier A = NULL, B = NULL, C = NULL;
	ByteCode bc = ByteCode_Create();
	bc->line_number = exe->line_number;
	if(IS_NULL(bc))
	{
		LOG_ERROR("ByteCode_Create() failed");
		return STATUS_FAILURE;
	}
	if(!IS_NULL(a))
	{
		A=Identifier_Clone(a);
	}
	if(!IS_NULL(b))
	{
		B=Identifier_Clone(b);
	}
	if(!IS_NULL(c))
	{
		C=Identifier_Clone(c);
	}
	switch(cmd)
	{
		case	SUB:
		case	ADD:
		case	MUL:
		case	DIV:
		case	MOD:
		case	GTH:
		case	LTH:
		case	GTE:
		case	LTE:
		case	CMP:
		case	NEQ:
		case	OR:
		case	AND:
		case	EQU:
			{
				bc->cmd = cmd;
				bc->A = A;
				bc->B = B;
				bc->C = C;
				break;
			}
		case NOT:
			{
				bc->cmd = NOT;
				bc->B = B;
				bc->C = C;
				break;
			}
		case JZ:
			{
				bc->cmd = JZ;
				bc->A = A;
				bc->u.label_number = number;
				break;
			}
		case JUMP:
			{
				bc->cmd = JUMP;
				bc->u.label_number = number;
				break;
			}
		case CALL:
			{
				bc->cmd = CALL;
				bc->A = A;
				bc->C = C;
				bc->u.num_arguments = number;
				break;
			}
		case LABEL:
			{
				Free(bc);
				bc = NULL;
				if(number >= exe->label_size)
				{
					exe->label_size = number + 50;
					exe->label_list = (unsigned int *)ReAlloc(exe->label_list, sizeof(unsigned int) * exe->label_size);
					if(IS_NULL(exe->label_list))
					{
						exe->label_list = NULL;
						LOG_ERROR("ReAlloc(%u) failed", sizeof(unsigned int) * exe->label_size);
						return STATUS_FAILURE;
					}
				}
				exe->label_list[number] = (unsigned int)exe->last;
				break;
			}
		case PUSH:
			{
				bc->cmd = PUSH;
				bc->A = A;
				break;
			}
		case VAR:
			{
				bc->cmd = VAR;
				bc->A = A;
				bc->B = B;
				break;
			}
		case FUNCTION:;
			{
				Free(bc);
				bc = NULL;
				if(FunctionList_AddLocalFunction(exe->func_list,A->u.str,(unsigned int)exe->last) != STATUS_SUCCESS)
				{
					LOG_ERROR("FunctionList_AddLocalFunction(%s) failed. Could be function '%s; already exist", A->u.str, A->u.str);
					Identifier_Destroy(A);
					return STATUS_FAILURE;
				}
				Identifier_Destroy(A);
				break;
			}
		case RETURN:
			{
				bc->cmd = RETURN;
				bc->A = A;
				break;
			}
		default:
			LOG_ERROR("unhandled cmd %u, aborting...", cmd);
			abort();
	}
	if(!IS_NULL(bc))
	{
		exe->last->next = bc;
		exe->last = bc;
	}
	return STATUS_SUCCESS;
}

