#include "Test.h"
#include "CSL.h"

TestClass::TestClass()
{
}

void TestClass::Start()
{
	API::CSL::Log("Start! Changed!");
}

void TestClass::Test()
{
	API::CSL::Log("Test!");
}
