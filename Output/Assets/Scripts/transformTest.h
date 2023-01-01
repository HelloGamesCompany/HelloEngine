#pragma once
#include "HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API.h"

class transformTest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	
	API_Transform transfromTestVariable;
};

HELLO_ENGINE_API_C transformTest* CreatetransformTest(ScriptToInspectorInterface* script)
{
	transformTest* classInstance = new transformTest();
	script->AddDragBoxTransform("transform dragging test: ", &classInstance->transfromTestVariable);

	return classInstance;
}