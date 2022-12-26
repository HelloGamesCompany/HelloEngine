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
	API::CSL::Log("Update with Late hot reload");
}

void TestClass::Test()
{
	API::CSL::Log("Test!");
}
