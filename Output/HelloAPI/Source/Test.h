#pragma once
#include "Macro.h"
#include "HelloBehavior.h"

class HELLO_ENGINE_API TestClass : HelloBehavior
{
public:

	TestClass();

	void Start() override;

	void Test();
};

HELLO_ENGINE_API_C TestClass* CreateTest()
{
	

	return new TestClass();
}