#pragma once
#include "HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

class Creationtest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
};

HELLO_ENGINE_API_C Creationtest* CreateCreationtest(ScriptToInspectorInterface* script)
{
	Creationtest* classInstance = new Creationtest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}