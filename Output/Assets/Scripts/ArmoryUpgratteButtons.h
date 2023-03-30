#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "ArmoryWeaponSelect.h"

#include "API/API.h"

class ArmoryUpgratteButtons : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton Unlock;
	API_UIButton Upgrate1;
	API_UIButton Upgrate2;
	API_UIButton Upgrate3;

	API_GameObject PanelUnlock;
	API_GameObject PanelUpgrate;

	API_GameObject currentWeapon;

	ArmoryWeaponSelect* WeaponInstance;

	bool isUnlocked;
};

