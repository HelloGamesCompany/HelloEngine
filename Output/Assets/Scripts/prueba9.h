#pragma once
#include "HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

class prueba9 : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
};

HELLO_ENGINE_API_C prueba9* Createprueba9(ScriptToInspectorInterface* script)
{
	prueba9* classInstance = new prueba9();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}