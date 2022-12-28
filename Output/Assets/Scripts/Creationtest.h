#pragma once
#include "HelloEngine/HelloBehavior.h"
#include "HelloEngine/ScriptToInspectorInterface.h"
#include "Macro.h"
class Creationtest : HelloBehavior
{
public:
void Start() override; 
void Update() override;

float creationTestFloat = 11.0f;
};
HELLO_ENGINE_API_C Creationtest* CreateCreationtest(ScriptToInspectorInterface* script)
{
	Creationtest* classInstance = new Creationtest();

	//script->AddDragFloat("creation test 2", &classInstance->creationTestFloat);

	return classInstance;
}