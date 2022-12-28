#pragma once
#include "HelloEngine/HelloBehavior.h"
#include "Macro.h"
class SerializationTesting : HelloBehavior
{
public:
void Start() override; 
void Update() override;
};
HELLO_ENGINE_API_C SerializationTesting* CreateSerializationTesting()
{
 return new SerializationTesting();
}