#include <malloc.h>
#include <string.h>
#include "util.h"

void* Malloc(size_t size)
{
	void *ptr;
	ptr = malloc(size);
	if(!IS_NULL(ptr))
	{
		memset(ptr, 0, size);
	}
	else
	{
		LOG_ERROR("malloc(%d) failed", size);
	}
	return ptr;
}
void Free(void *ptr)
{
	if(!IS_NULL(ptr))
	{
		free(ptr);
	}
}
void* ReAlloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}
