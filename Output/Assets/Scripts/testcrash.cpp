#include "testcrash.h"

HELLO_ENGINE_API_C testcrash* Createtestcrash(ScriptToInspectorInterface* script)
{
	testcrash* classInstance = new testcrash();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void testcrash::Start()
{

}
void testcrash::Update()
{

}