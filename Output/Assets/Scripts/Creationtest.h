#pragma once
#include "HelloEngine/HelloBehavior.h"
#include "HelloEngine/ScriptToInspectorInterface.h"
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

return classInstance;
}