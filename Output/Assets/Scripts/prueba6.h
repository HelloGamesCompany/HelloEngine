#pragma once
#include "HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

class prueba6 : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
};

HELLO_ENGINE_API_C prueba6* Createprueba6(ScriptToInspectorInterface* script)
{
	prueba6* classInstance = new prueba6();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}