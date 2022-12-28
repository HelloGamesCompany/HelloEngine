#pragma once
#include "HelloEngine/HelloBehavior.h"
#include "HelloEngine/ScriptToInspectorInterface.h"
#include "Macro.h"
class testScript : HelloBehavior
{
public:
void Start() override; 
void Update() override;
};
HELLO_ENGINE_API_C testScript* CreatetestScript(ScriptToInspectorInterface* script)
{
testScript* classInstance = new testScript();

return classInstance;
}