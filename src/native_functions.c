
#include <string.h>
#include "native_functions.h"
#include "variable_list.h"
/*
 * To avoid memory leaks please follow the following rules.
 * Every return value should be created here or cloned here
 */

Identifier Function_Output(Identifier *args, int num_args)
{
	int i;
	for(i = 1; i <= num_args; i++)
	{
		switch(args[i]->type)
		{
			case IDENTIFIER_TYPE_NUMBER:
					printf("%d", args[i]->u.number);
					break;
			case IDENTIFIER_TYPE_FLOAT:
					printf("%lf", args[i]->u.real);
					break;
			case IDENTIFIER_TYPE_STRING:
					printf("%s", args[i]->u.str);
					break;
			default:
					printf("unknown id type '%d'", args[i]->type);
					abort();
		}
	}
	printf("\n");
	return NULL;
}

Identifier Function_Max(Identifier *args, int num_args)
{
	int i, max = 0;
	for(i = 1; i <= num_args; i++)
	{
		switch(args[i]->type)
		{
			case IDENTIFIER_TYPE_NUMBER:
					if(max < args[i]->u.number)
					{
						max = args[i]->u.number;
					}
					break;

			case IDENTIFIER_TYPE_FLOAT:
			case IDENTIFIER_TYPE_STRING:
					LOG_ERROR("invalid argument to Max() function of type string");
					break;
			default:
					printf("unknown id type '%d'", args[i]->type);
					abort();
		}
	}
	return Identifier_NewInteger(max);
}

Identifier Function_TypeOf(Identifier *args, int num_args)
{
	if(args[0]->u.number != 1)
	{
		LOG_ERROR("typeof(variable) usage");
		return Identifier_NewInteger(0);
	}
	if(args[1]->type != IDENTIFIER_TYPE_OBJECT)
		return (Identifier_NewString(IdentifierType_to_str(args[1]->type)));
	else
		return Identifier_NewString(args[1]->u.obj->type);
}

#ifdef ENABLE_KEY_VALUE

typedef struct _KeyValue
{
	VariableList vl;
	VariableList cur_ptr;
}*KeyValue;

void KeyValue_delete(void *ptr)
{
	KeyValue keyValue = (KeyValue)ptr;
	VariableList_Destroy(keyValue->vl);
	memset(keyValue, 0, sizeof(struct _KeyValue));
	Free(keyValue);
}

Identifier Function_KeyValue(Identifier *args, int num_args)
{
	Object 		obj = NULL;
	Identifier 	i = NULL;
	KeyValue	keyValue;

	keyValue = (KeyValue)Malloc(sizeof(struct _KeyValue));
	if(IS_NULL(keyValue))
	{
		LOG_ERROR("Malloc(%u) failed", sizeof(struct _KeyValue));
		return Identifier_NewInteger(0);
	}
	keyValue->vl= VariableList_Create();
	if(!IS_NULL(keyValue->vl))
	{
		obj = (Object)Malloc(sizeof(struct _Object));
		memset(obj, 0, sizeof(struct _Object));
		obj->priv_data = (void*)keyValue;
		obj->obj_delete = KeyValue_delete;
		strcpy(obj->type, "keyvalue");
		i = Identifier_NewObject(obj);
	}
	return i;
}

Identifier Function_KeyValueAdd(Identifier *args, int num_args)
{
	KeyValue keyValue;
	if(args[0]->u.number != 3 || args[1]->type != IDENTIFIER_TYPE_OBJECT || args[2]->type != IDENTIFIER_TYPE_STRING)
	{
		LOG_ERROR("KeyValueAdd(object, key, value) usage");
		return Identifier_NewInteger(0);
	}
	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		Identifier i;
		i = VariableList_FindVariable(keyValue->vl, args[2]->u.str);
		if(IS_NULL(i))
		{
			if(STATUS_SUCCESS == VariableList_AddVariable(keyValue->vl, args[2]->u.str, args[3]))
			{
				return Identifier_NewInteger(1);
			}
		}
		else
		{
			Identifier_Copy(i, args[3]);
		}
	}
	return Identifier_NewInteger(0);
}

Identifier Function_KeyValueGet(Identifier *args, int num_args)
{
	Identifier i = NULL;
	KeyValue keyValue;
	if(args[0]->u.number != 2 || args[1]->type != IDENTIFIER_TYPE_OBJECT || args[2]->type != IDENTIFIER_TYPE_STRING)
	{
		LOG_ERROR("KeyValueAdd(object, key, value) usage");
		return Identifier_NewInteger(0);
	}
	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		i = VariableList_FindVariable(keyValue->vl, args[2]->u.str);
		if(IS_NULL(i))
		{
			return Identifier_NewInteger(0);
		}
	}
	return Identifier_Clone(i);
}


Identifier Function_KeyValueIterator(Identifier *args, int num_args)
{
	KeyValue keyValue;
	if(args[0]->u.number != 1 || args[1]->type != IDENTIFIER_TYPE_OBJECT)
	{
		LOG_ERROR("KeyValueIterator(object) usage");
		return Identifier_NewInteger(0);
	}

	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		keyValue->cur_ptr = keyValue->vl;
		return Identifier_NewInteger(1);
	}
	return Identifier_NewInteger(0);
}
Identifier Function_KeyValueNext(Identifier *args, int num_args)
{
	KeyValue keyValue;
	if(args[0]->u.number != 1 || args[1]->type != IDENTIFIER_TYPE_OBJECT)
	{
		LOG_ERROR("KeyValueNext(object) usage");
		return Identifier_NewInteger(0);
	}

	keyValue = (KeyValue)args[1]->u.obj->priv_data;
	if(!IS_NULL(keyValue))
	{
		if(IS_NULL(keyValue->cur_ptr))
		{
			LOG_ERROR("First call KeyValueIterator() and then call KeyValueNext");
			return Identifier_NewInteger(0);
		}
		keyValue->cur_ptr = keyValue->cur_ptr->next;

		if(!IS_NULL(keyValue->cur_ptr))
		{
			Identifier obj = Map_Create();
			Map_AddString(obj,"key", keyValue->cur_ptr->variable_name);
			Map_AddElement(obj,"value", keyValue->cur_ptr->id);
			return obj;
		}
	}
	return Identifier_NewInteger(0);
}

#endif




#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
typedef struct _filelist_
{
	char *path;
	struct dirent *d;
	DIR *dir;
}*FileList;

void FileList_Delete(void *obj)
{
	FileList fl = (FileList)obj;
	closedir(fl->dir);
	Free(fl->path);
	memset(fl, 0, sizeof(struct _filelist_));
	Free(fl);
}

Identifier Function_FileListObject(Identifier *args, int num_args)
{
	Object obj = NULL;
	FileList fl = NULL;
	if(num_args != 1 && args[1]->type != IDENTIFIER_TYPE_STRING)
	{
		LOG_ERROR("Usage: FileListObject(pathname)");
		return Identifier_NewInteger(0);
	}
	fl = (FileList)Malloc(sizeof(struct _filelist_));

	if(!IS_NULL(fl))
	{
		char *str = args[1]->u.str;
		if(str[strlen(str)-1] == '/')
			str[strlen(str)-1]=0;
		fl->path = (char*)Malloc(strlen(args[1]->u.str)+1);
		strcpy(fl->path, args[1]->u.str);
		fl->dir = opendir(fl->path);
		if(IS_NULL(fl->dir))
		{
			Free(fl->path);
			Free(fl);
			return Identifier_NewInteger(0);
		}
		obj = (Object)Malloc(sizeof(struct _Object));
		memset(obj, 0, sizeof(struct _Object));
		obj->priv_data = (void*)fl;
		obj->obj_delete = FileList_Delete;
		strcpy(obj->type, "filelist");
		return Identifier_NewObject(obj);
	}
	return Identifier_NewInteger(0);
}

Identifier Function_FileListGet(Identifier *args, int num_args)
{
	FileList fl = NULL;
	if(args[0]->u.number != 1 || args[1]->type != IDENTIFIER_TYPE_OBJECT || (strcmp(args[1]->u.obj->type, "filelist") != 0))
	{
		LOG_ERROR("KeyValueIterator(filelist Object) usage");
		return Identifier_NewInteger(0);
	}
	fl = (FileList)args[1]->u.obj->priv_data;
	fl->d = readdir(fl->dir);
	if(!IS_NULL(fl->d))
	{
		struct stat st;
		char full_path[128];
		snprintf(full_path, 128, "%s/%s", fl->path, fl->d->d_name);
		if(stat(full_path, &st) == 0)
		{
			Identifier obj = Map_Create();
			Map_AddString(obj,"name", full_path);
			Map_AddInt(obj,"size", (int)st.st_size);
			Map_AddInt(obj, "isdir", (int)S_ISDIR(st.st_mode));
			Map_AddInt(obj, "readable", (int)(st.st_mode & S_IRUSR));
			Map_AddInt(obj, "writable", (int)(st.st_mode & S_IWUSR));
			Map_AddInt(obj, "executable", (int)(st.st_mode & S_IXUSR));
			return obj;
		}
	}
	return Identifier_NewInteger(0);
}

void Register_Native_Functions(Executable exe)
{
	Executable_AddNativeFunction(exe, "output", Function_Output);
	Executable_AddNativeFunction(exe, "max", Function_Max);
	Executable_AddNativeFunction(exe, "typeof", Function_TypeOf);
#ifdef ENABLE_KEY_VALUE
	Executable_AddNativeFunction(exe, "KeyValue", Function_KeyValue);
	Executable_AddNativeFunction(exe, "KeyValueAdd", Function_KeyValueAdd);
	Executable_AddNativeFunction(exe, "KeyValueGet", Function_KeyValueGet);
	Executable_AddNativeFunction(exe, "KeyValueIterator", Function_KeyValueIterator);
	Executable_AddNativeFunction(exe, "KeyValueNext", Function_KeyValueNext);
#endif


	Executable_AddNativeFunction(exe, "FileListObject", Function_FileListObject);
	Executable_AddNativeFunction(exe, "FileListGet", Function_FileListGet);
}
