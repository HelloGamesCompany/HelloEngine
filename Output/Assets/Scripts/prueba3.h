#pragma once
#include "HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

class prueba3 : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
};

HELLO_ENGINE_API_C prueba3* Createprueba3(ScriptToInspectorInterface* script)
{
	prueba3* classInstance = new prueba3();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}