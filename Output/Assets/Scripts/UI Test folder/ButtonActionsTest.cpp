#include "ButtonActionsTest.h"
HELLO_ENGINE_API_C ButtonActionsTest* CreateButtonActionsTest(ScriptToInspectorInterface* script)
{
	ButtonActionsTest* classInstance = new ButtonActionsTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ButtonActionsTest::Start()
{
	Console::Log("Button started!");
	gameObject.GetTransform().SetScale(API_Vector3(1.f, 1.f, 1.f));
}
void ButtonActionsTest::Update()
{
	
}