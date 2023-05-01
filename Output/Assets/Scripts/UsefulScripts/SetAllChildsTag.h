#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class SetAllChildsTag : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_String tag;
	bool onlyDirectChilds;
};

