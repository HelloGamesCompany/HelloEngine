#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ArmoryWeaponSelect : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton Weapon1;
	API_UIButton Weapon2;
	API_UIButton Weapon3;
	API_UIButton Weapon4;
	API_UIButton Weapon5;
	API_UIButton Weapon6;
	API_UIButton Weapon7;
	API_UIButton Weapon8;
};

