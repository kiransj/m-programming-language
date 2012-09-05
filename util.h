#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>

#define LOG_INFO(format, args...)          fprintf(stdout, "%s-%d:" format "\n",  __FILE__,  __LINE__,  ## args)
#define LOG_ERROR(format, args...)         fprintf(stdout, "%s-%d:" format "\n",  __FILE__,  __LINE__,  ## args)
#define LOG_INFO_NL(format, args...)       fprintf(stdout, format "\n",  ## args)


void* Malloc(size_t size);
void* ReAlloc(void *ptr, size_t size);

void Free(void *ptr);

#define IS_NULL(x) (NULL == (x))

typedef enum
{
	STATUS_SUCCESS = 0,
	STATUS_FAILURE
}STATUS;

#endif
