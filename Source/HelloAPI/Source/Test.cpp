#include "Test.h"
#include "CSL.h"

TestClass::TestClass()
{
}

void TestClass::Start()
{
	API::CSL::Log("Start!");
}

void TestClass::Test()
{
	API::CSL::Log("Test!");
}
