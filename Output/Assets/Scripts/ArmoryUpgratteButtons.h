#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIInput.h"
#include "ArmoryWeaponSelect.h"

class ArmoryUpgratteButtons : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Upgrade1();
	void Upgrade2();
	void Upgrade3();

	API_UIButton Unlock;
	API_UIButton Upgrate1;
	API_UIButton Upgrate2;
	API_UIButton Upgrate3;

	API_GameObject PanelUpgrateW1;

	API_UIInput PanelUnlockW2;
	API_UIInput PanelUpgrateW2;

	API_GameObject PanelUnlockW3;
	API_GameObject PanelUpgrateW3;

	API_GameObject PanelUnlockW4;
	API_GameObject PanelUpgrateW4;

	API_GameObject PanelUnlockW5;
	API_GameObject PanelUpgrateW5;

	API_GameObject PanelUnlockW6;
	API_GameObject PanelUpgrateW6;

	API_GameObject currentWeapon;
	API_UIInput SelectWeaponList;

	ArmoryWeaponSelect* weaponInstance;

	bool isUnlocked;
	bool upgrade1;
	bool upgrade2;

	int oneTime;
};

