#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

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

	API_GameObject PanelUpgrateW1;

	API_GameObject PanelUnlockW2;
	API_GameObject PanelUpgrateW2;

	API_GameObject PanelUnlockW3;
	API_GameObject PanelUpgrateW3;

	API_GameObject PanelUnlockW4;
	API_GameObject PanelUpgrateW4;

	API_GameObject PanelUnlockW5;
	API_GameObject PanelUpgrateW5;

	API_GameObject PanelUnlockW6;
	API_GameObject PanelUpgrateW6;

	API_GameObject currentWeapon;

	///ArmoryWeaponSelect* WeaponInstance = nullptr;

	bool isUnlocked;
};

