#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class AbilityTreeScript : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	API_UIButton AbilityList1;
	API_UIButton AbilityList2;
	API_UIButton AbilityList3;
	API_UIButton AbilityList4;

	API_GameObject PanelAbility1;
	API_GameObject PanelAbility2;
	API_GameObject PanelAbility3;
	API_GameObject PanelAbility4;
};
