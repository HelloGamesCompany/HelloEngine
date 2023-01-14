#include "InspectorTest.h"
HELLO_ENGINE_API_C InspectorTest* CreateInspectorTest(ScriptToInspectorInterface* script)
{
	InspectorTest* classInstance = new InspectorTest();

	script->AddDragFloat("Float", &classInstance->testFloat);
	script->AddDragInt("Int", &classInstance->testInt);
	script->AddCheckBox("Bool", &classInstance->testBool);
	script->AddDragBoxGameObject("Game Object", &classInstance->testGameObject);

	return classInstance;
}

void InspectorTest::Start()
{
	Console::Log("Late Start!");

}
void InspectorTest::Update()
{
	Console::Log("Test change release!!!!: ");
}