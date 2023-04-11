#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIInput.h"

class AbilitySelect : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton abilitySelect;
	API_UIInput abilityTypePanel;
	API_UIInput abilityPanel;

	int setEnablePanel = false;
};

