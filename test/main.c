#include "M.h"

int main(int argc, char *argv[])
{
	Executable exe;
	STATUS ret;
	if(argc == 1)
	{
		LOG_ERROR("usage %s <filename>", argv[0]);
		return 1;
	}
	exe = Executable_Create();

	if(!IS_NULL(exe))
	{
		ret = Compile(exe, argv[1]);
		if(STATUS_SUCCESS == ret)
		{
			if(0 && ExecutionContext_Execute(exe, "Main") == STATUS_SUCCESS)
			{
				LOG_INFO("program returned : %d", ExecutionContext_GetReturnValue(exe));
			}
		}
		Executable_Destroy(exe);
	}

	return 0;
}

#if 0
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
int main(int argc, char *argv[])
{
	struct dirent *d;
	DIR *dir;

	dir = opendir("/bin");
	if(NULL != dir)
	{
		while(NULL != (d = readdir(dir)))
		{
			char full_path[128];
			struct stat st;			
			snprintf(full_path, 128, "/bin/%s", d->d_name);
			if(stat(full_path, &st) == 0)
			{
				LOG_INFO_NL("%32s %u", full_path, (unsigned int)st.st_size);
			}
		}
		closedir(dir);
	}
	return 0;
}
#endif
