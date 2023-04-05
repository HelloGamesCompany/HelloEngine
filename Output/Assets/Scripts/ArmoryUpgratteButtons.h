#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIInput.h"
#include "API/API_UIText.h"
#include "ArmoryWeaponSelect.h"

class ArmoryUpgratteButtons : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Upgrade1();
	void Upgrade2();
	void Upgrade3();

	API_UIButton Upgrate1;
	API_UIButton Upgrate2;
	API_UIButton Upgrate3;

	API_UIInput PanelUpgrate;

	API_GameObject currentWeapon;
	API_UIInput SelectWeaponList;

	API_UIText bluePrintText;

	ArmoryWeaponSelect* weaponInstance;

	bool isUnlocked;
	bool upgrade1;
	bool upgrade2;
};

