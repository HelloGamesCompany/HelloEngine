#pragma once
#include "HelloEngine/HelloBehavior.h"
#include "HelloEngine/ScriptToInspectorInterface.h"
#include "Macro.h"
class Creationtest : HelloBehavior
{
public:
void Start() override;
void Update() override;
public:
	float creationTestFloat = 1.0f;
	int intTest = 5;
	bool boolTest = false;
	std::string stringTest = "Hello world.";
};
HELLO_ENGINE_API_C Creationtest* CreateCreationtest(ScriptToInspectorInterface* script)
{
Creationtest* classInstance = new Creationtest();

	script->AddDragFloat("creation test", &classInstance->creationTestFloat);
	script->AddDragInt("int test", &classInstance->intTest);
	script->AddCheckBox("bool test", &classInstance->boolTest);
	script->AddInputBox("string test", &classInstance->stringTest);

	return classInstance;
}
