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

typedef struct _map
{
	VariableList list;
}*Map;

Identifier Map_Create(void);
Identifier Map_FindElement(Identifier m, const char *element_name);

STATUS Map_AddElement(Identifier Obj, const char *element_name, Identifier value);
STATUS Map_AddInt(Identifier Obj, const char *element_name, const int num);
STATUS Map_AddString(Identifier Obj, const char *element_name, const char *string);
#endif
