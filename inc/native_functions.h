#ifndef _NATIVE_FUNCTIONS_H_

#include "M.h"

typedef Identifier (*NativeFunction)(Identifier *args, int num_args);
STATUS Executable_AddNativeFunction(Executable exe, const char *func_name, NativeFunction nf);
void Register_Native_Functions(Executable exe);
Identifier Function_Output(Identifier *args, int num_args);
Identifier Function_Max(Identifier *args, int num_args);
Identifier Function_TypeOf(Identifier *args, int num_args);


#ifdef ENABLE_KEY_VALUE
Identifier Function_KeyValue(Identifier *args, int num_args);
Identifier Function_KeyValueAdd(Identifier *args, int num_args);
Identifier Function_KeyValueGet(Identifier *args, int num_args);
Identifier Function_KeyValueIterator(Identifier *args, int num_args);
Identifier Function_KeyValueNext(Identifier *args, int num_args);
Identifier Function_KeyValueGetKey(Identifier *args, int num_args);
Identifier Function_KeyValueGetValue(Identifier *args, int num_args);
#endif

#endif
