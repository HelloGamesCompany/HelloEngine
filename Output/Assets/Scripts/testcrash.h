#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

class testcrash : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
};

