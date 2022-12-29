#pragma once
#include "HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
class testScript : HelloBehavior
{
public:
void Start() override; 
void Update() override;

int intTest = 5;
};
HELLO_ENGINE_API_C testScript* CreatetestScript(ScriptToInspectorInterface* script)
{
	testScript* classInstance = new testScript();
	script->AddDragInt("int test 2", &classInstance->intTest);
	return classInstance;
}