#include "Test.h"
#include "CSL.h"

TestClass::TestClass()
{
}

void TestClass::Start()
{
	API::CSL::Log("Start! Changed!");
}

void TestClass::Update()
{
	API::CSL::Log("The DLL works! And  LATE hot reloading too!!");
}

void TestClass::Test()
{
	API::CSL::Log("Test!");
}
