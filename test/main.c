
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
	ret = Compile(exe, argv[1]);
	if(STATUS_SUCCESS == ret)
	{
		if(ExecutionContext_Execute(exe, "Main") == STATUS_SUCCESS)
		{
		}
		Executable_Destroy(exe);
	}

	return 0;
}


