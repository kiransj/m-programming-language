#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "M.h"
Executable Execute(char *filename)
{
	Executable exe;
	STATUS ret;

	exe = Executable_Create();

	if(!IS_NULL(exe))
	{
		ret = Compile(exe, filename);
		if(STATUS_SUCCESS == ret)
		{
			return exe;
		}
		Executable_Destroy(exe);
		exe = NULL;
	}

	return exe;
}

int main(int argc, char *argv[])
{
	struct dirent *d;
	DIR *dir;
	Executable exe;
	Identifier obj;

	obj = Map_Create();
	exe = Execute(argv[1]);

	dir = opendir("/bin/");
	if(NULL != dir)
	{
		while(NULL != (d = readdir(dir)))
		{
			char full_path[128];
			struct stat st;			
			snprintf(full_path, 128, "/bin/%s", d->d_name);
			if(stat(full_path, &st) == 0)
			{
				Map_AddString(obj,"name", full_path);
				Map_AddInt(obj,"size", (int)st.st_size);

				if(ExecutionContext_Execute(exe, "Main", obj) == STATUS_SUCCESS)
				{

				}
			}
		}
		closedir(dir);
	}
	
	Identifier_Destroy(obj);
	Executable_Destroy(exe);
	return 0;
}

