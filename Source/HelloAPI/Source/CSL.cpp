#include "CSL.h"
#include <windows.h>

typedef void(*FunctionPtr)();

void API::CSL::Log(const std::string text)
{
	HMODULE hmodule = GetModuleHandle(0);
	FunctionPtr f = (FunctionPtr)GetProcAddress(hmodule, "GetFunctionTest::TestFunction");

	if (f)
	{
		f();
	}
	else
	{
		printf("Not found function");
	}
}

