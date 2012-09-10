
#include "compiler.h"
#include "executable.h"
#include "util.h"

int main(int argc, char *argv[])
{
	Executable exe;

	if(argc == 1)
	{
		LOG_ERROR("usage %s <filename>", argv[0]);
		return 1;
	}
	exe = Compile(argv[1]);
	if(!IS_NULL(exe))
	{
		if(ExecutionContext_Execute(exe, "Main") == STATUS_SUCCESS)
		{
			char buf[64];
			Identifier_to_str(exe->ret_value, buf, 64);
			LOG_INFO("program returned %s", buf);
		}
		Executable_Destroy(exe);
	}

	return 0;
}


