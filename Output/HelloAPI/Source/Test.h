#pragma once
#include "Macro.h"
#include "HelloEngine/HelloBehavior.h"

class HELLO_ENGINE_API TestClass : HelloBehavior
 {
 public:

	TestClass();
	virtual ~TestClass() {};

	void Start();

	void Update();

	void Test();
};

HELLO_ENGINE_API_C TestClass* CreateTest()
{
	

	return new TestClass();
}