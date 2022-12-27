#pragma once
#include "HelloEngine/HelloBehavior.h"
#include "Macro.h"
class NewScript : HelloBehavior
{
public:
void Start() override; 
void Update() override;
};
HELLO_ENGINE_API_C NewScript* CreateNewScript()
{
 return new NewScript();
}