#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class DashUI : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void Dash();

	uint Texture_Dash_on;
	uint Texture_Dash_off;
	//API_Material Material_Dash;
	bool activeDash;

	float cooldown;
	float cooldownBetweenDash;
	float coolDownBetweenDashValue;
	float cooldownValue;

};

