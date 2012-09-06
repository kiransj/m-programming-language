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
			Free(exe);
			return NULL;
		}
	}
	else
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(struct _Executable));
	}
	return exe;
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
	Free(exe);
}

STATUS Executable_AddCmd(Executable exe, CompilerCmd cmd, Identifier a, Identifier b, Identifier c, int number)
{
	Identifier A, B, C;
	ByteCode bc = ByteCode_Create();
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
				bc->u.num_arguments = number;
				break;
			}
		case ARGUMENT:
			{
				bc->cmd = ARGUMENT;
				bc->A = A;
				bc->u.argument_pos = number;
				break;
			}
		case LABEL:
			{
				Free(bc);
				bc = NULL;
				exe->label_list[exe->label_index++] = (unsigned int)exe->last;
				if(exe->label_index == exe->label_size)
				{
					exe->label_size += 25;
					exe->label_list = (unsigned int *)ReAlloc(exe->label_list, sizeof(unsigned int) * exe->label_size);
					if(IS_NULL(exe->label_list))
					{
						exe->label_list = NULL;
						LOG_ERROR("ReAlloc(%u) failed", sizeof(unsigned int) * exe->label_size);
						return STATUS_FAILURE;
					}
				}
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

