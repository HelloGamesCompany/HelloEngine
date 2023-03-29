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

	API_Material SelectedWeapon;
	uint Weapon1Texture;
	uint Weapon2Texture;
	uint Weapon3Texture;
	uint Weapon4Texture;
	uint Weapon5Texture;
	uint Weapon6Texture;

};

