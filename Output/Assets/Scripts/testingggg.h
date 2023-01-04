#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

class testingggg : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
};

HELLO_ENGINE_API_C testingggg* Createtestingggg(ScriptToInspectorInterface* script)
{
	testingggg* classInstance = new testingggg();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}