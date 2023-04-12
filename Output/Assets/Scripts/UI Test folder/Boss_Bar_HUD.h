#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API_UIImage.h"

#include "API/API.h"

class Boss_Bar_HUD : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Boss_Bar(float HP_Boss_Value, bool Is_Active);

	API_GameObject boss_HUD;
	API_UIImage boss_bar;
};

