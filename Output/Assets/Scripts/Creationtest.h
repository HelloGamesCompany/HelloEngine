#pragma once
#include "HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API.h"

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
	API_GameObject testGameObject;
	API_Transform testTransform;
};

HELLO_ENGINE_API_C Creationtest* CreateCreationtest(ScriptToInspectorInterface* script)
{
Creationtest* classInstance = new Creationtest();

	script->AddDragFloat("creation test", &classInstance->creationTestFloat);
	script->AddDragInt("int test 2", &classInstance->intTest);
	script->AddInputBox("string test", &classInstance->stringTest);

	return classInstance;
}
