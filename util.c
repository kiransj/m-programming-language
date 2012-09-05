#include <malloc.h>
#include "util.h"

void* Malloc(size_t size)
{
	return malloc(size);
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
