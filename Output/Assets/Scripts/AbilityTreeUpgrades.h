#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class AbilityTreeUpgrades : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton Ability1;
	API_UIButton Ability2;
	API_UIButton Ability3;
	API_UIButton Ability4;
};

