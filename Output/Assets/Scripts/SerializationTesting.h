#pragma once
#include "HelloEngine/HelloBehavior.h"
#include "HelloEngine/ScriptToInspectorInterface.h"
#include "Macro.h"

class SerializationTesting : HelloBehavior
{
public:
void Start() override; 
void Update() override;

public:
	float floatFromScript = 1.0f;

};
HELLO_ENGINE_API_C SerializationTesting* CreateSerializationTesting(ScriptToInspectorInterface* script)
{
	SerializationTesting* classInstance = new SerializationTesting();

	script->AddDragFloat("Float from Script", &classInstance->floatFromScript);
	
	return classInstance;
}