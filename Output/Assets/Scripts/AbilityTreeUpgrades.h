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

	API_UIButton buttonHovered;

	API_GameObject description;
	API_GameObject descriptionBack;
	API_GameObject descriptionFoward;
};

