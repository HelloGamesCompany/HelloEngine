#pragma once
#include "HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

class prueba10 : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
};

HELLO_ENGINE_API_C prueba10* Createprueba10(ScriptToInspectorInterface* script)
{
	prueba10* classInstance = new prueba10();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}