#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Creationtest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
};

