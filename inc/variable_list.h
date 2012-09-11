#ifndef _VARIABLE_LIST_H_
#define _VARIABLE_LIST_H_

#include "identifier.h"

struct _variablelist;
typedef struct _variablelist *VariableList;

struct _variablelist
{
	char *variable_name;
	Identifier id;
	struct _variablelist *next;
};


VariableList VariableList_Create(void);
void 		 VariableList_Destroy(VariableList);
STATUS 		 VariableList_AddVariable(VariableList vl, const char *variable_name, Identifier v);
Identifier   VariableList_FindVariable(VariableList, const char *variable_name);
#endif
