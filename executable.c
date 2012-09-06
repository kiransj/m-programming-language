#include <string.h>
#include "executable.h"


static ByteCode ByteCode_CreateTmp(void)
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

		exe->first = exe->last = ByteCode_CreateTmp();
		if(IS_NULL(exe->first))
		{
			LOG_ERROR("ByteCode_CreateTmp() failed");
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


STATUS Executable_AddCmd(Executable exe, CompilerCmd cmd, Identifier A, Identifier B, Identifier C, int number)
{
	ByteCode bc = ByteCode_CreateTmp();
	if(IS_NULL(bc))
	{
		LOG_ERROR("ByteCode_CreateTmp() failed");
		return STATUS_FAILURE;
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
		case LABEL:
			{
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
	return STATUS_SUCCESS;
}
