#include "testdebug2.h"
HELLO_ENGINE_API_C testdebug2* Createtestdebug2(ScriptToInspectorInterface* script)
{
	testdebug2* classInstance = new testdebug2();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void testdebug2::Start()
{

}
void testdebug2::Update()
{

}