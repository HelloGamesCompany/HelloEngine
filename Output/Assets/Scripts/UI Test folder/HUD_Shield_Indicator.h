#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class HUD_Shield_Indicator : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void isShieldEnable(bool Shieldpowerup_True);

	API_GameObject Shield;
	bool shield_Enable = false;
};

