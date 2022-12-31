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

	float creationTestFloat = 5.0f;
	int intTest = 10;
	API_GameObject testGameObject;
};

HELLO_ENGINE_API_C Creationtest* CreateCreationtest(ScriptToInspectorInterface* script)
{
Creationtest* classInstance = new Creationtest();

	script->AddDragFloat("creation test", &classInstance->creationTestFloat);
	script->AddDragInt("int test 2", &classInstance->intTest);
	script->AddDragBoxGameObject("test game object", &classInstance->testGameObject);

	return classInstance;
}