#include "Test.h"
#include "CSL.h"

TestClass::TestClass()
{
}

void TestClass::Start()
{
	API::CSL::Log("Start! From DLL!!");
}

void TestClass::Test()
{
	API::CSL::Log("Test!");
}
