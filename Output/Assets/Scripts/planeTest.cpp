#include "planeTest.h"
HELLO_ENGINE_API_C planeTest* CreateplaneTest(ScriptToInspectorInterface* script)
{
	planeTest* classInstance = new planeTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void planeTest::Start()
{
	Console::Log("Plane");
}
void planeTest::Update()
{

}