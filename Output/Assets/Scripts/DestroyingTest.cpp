#include "DestroyingTest.h"
HELLO_ENGINE_API_C DestroyingTest* CreateDestroyingTest(ScriptToInspectorInterface* script)
{
	DestroyingTest* classInstance = new DestroyingTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void DestroyingTest::Start()
{
	Console::Log("Start!");
}
void DestroyingTest::Update()
{

}