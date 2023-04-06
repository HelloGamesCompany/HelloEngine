#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API_UIText.h"

#include "API/API.h"

class UI_Municion : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	int hola = 100;

	API_GameObject TEXT_munition_InfinitWeapon_On;
	API_GameObject TEXT_munition_InfinitWeapon_Off;
	API_UIText TEXT_munition_Normal;
	API_UIText TEXT_munition_Special;
};

